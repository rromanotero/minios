#include "system.h"
#include "hal/hal.h"
#include "loader.h"
#include "utils/stack.h"
#include "scheduler.h"
#include "scheduling_policy.h"

#define TICK_FREQ		3
#define CONTEXT_SIZE    16
#define INITIAL_APSR    (1 << 24) //Bit 24 is the Thumb bit
#define OFFSET_LR       13
#define OFFSET_PC       14
#define OFFSET_APSR     15

__attribute__((naked)) static void tick_callback(void);
__attribute__((naked)) static void low_pty_callback(void);

extern void process_thread_delete(void);
extern void idle_process_thread(void);

static tProcessList proc_list;			// process list
static tMiniProcess* wait_list[10];		//waiting list
static tMiniProcess* active_proc;		//The active process
static uint32_t proc_count = 0;

//A null process (used to mark the lack of an active process)
static tMiniProcess null_proc = { 
			.name = "null", 
			.state = ProcessStateNull
	};			

/*
*	Scheduler Init
*  
*   Initializes the scheduler. The system timer is not started here.
*/
void scheduler_init(void){
	//Init process stack
	static tMemRegion stack_memreg;
	hal_memreg_read( MemRegUserStack, &stack_memreg );
	stack_init( stack_memreg.base );	//stack_init( epstack )
	
	//Inits Low Pty Int
	hal_cpu_lowpty_softint_register_callback( low_pty_callback );
	
	//Active process is the null process
	active_proc = &null_proc;
	
	//No processes
	proc_count = 0;
	
	//Create idle process/thread
	//(we need one!)
	scheduler_thread_create( idle_process_thread, "idle process thread", 256 );
}

/*
*	The infamous tick callback
*
*	Context switch takes place here.
*/
__attribute__((naked)) static void tick_callback(void){
	
	//save software context
	hal_cpu_save_context();
	
	//Not the null process?
	//(this'll skiip hal_cpu_get_psp() on the very first tick)
	if( active_proc->state != ProcessStateNull ){
		//save SP
		active_proc->sp = hal_cpu_get_psp();
	}
	
	//get next active process
	active_proc = scheduling_policy_next( active_proc, &proc_list );
	
	//restore SP
	hal_cpu_set_psp( active_proc->sp );
	
	//restore software context
	hal_cpu_restore_context();
	
	//give CPU to active process
	hal_cpu_return_exception_user_mode();
}

/*
*	Low Priority Callback
*	(This callback is interruptible by the system timer)
*
*	Context switch takes place here.
*/
__attribute__((naked)) static void low_pty_callback(void){
	
	//save software context
	hal_cpu_save_context();
	
	//stop systick (else race conditions)
	hal_cpu_systimer_stop();
	
	//Not the null process?
	//(this'll skiip hal_cpu_get_psp() on the very first tick)
	if( active_proc->state != ProcessStateNull ){
		//save SP
		active_proc->sp = hal_cpu_get_psp();
	}
	
	//get next active process
	active_proc = scheduling_policy_next( active_proc, &proc_list );
	
	//restore SP
	hal_cpu_set_psp( active_proc->sp );
	
	//restart system timer (from 0)
	hal_cpu_systimer_reestart();
	
	//restore software context
	hal_cpu_restore_context();
	
	//give CPU to active process
	hal_cpu_return_exception_user_mode();
}

/*
*	Scheduler Process Create
*
*	Creates a process from a binary in nvmem. Ticking here begins!
*/
uint32_t scheduler_process_create( uint8_t* binary_file_name, uint8_t* name, uint32_t* loader_rval ){	
	tMemRegion proc_memregion;
	uint32_t stack_sz;
	
	//Load app binary
	uint32_t rval = loader_load_app( binary_file_name, &proc_memregion, &stack_sz );
	if(  rval != LOADER_LOAD_SUCCESS ){
		*loader_rval = rval;				//populate loader error
		return SCHEDULER_PROCESS_CREATE_FAILED;
	}

	//Set process info
	proc_list.list[proc_list.count].name = name;
	proc_list.list[proc_list.count].state = ProcessStateReady;
	
	//Allocate space for "fake" context
	stack_alloc( CONTEXT_SIZE );
	
	//Allocate space for remaining stack
	proc_list.list[proc_list.count].sp = stack_top();       //set SP
	stack_alloc( stack_sz - CONTEXT_SIZE );					//make space 
	
	//Insert "fake" context
	proc_list.list[proc_list.count].sp[OFFSET_LR] =     ((uint32_t) (process_thread_delete +1));
	proc_list.list[proc_list.count].sp[OFFSET_PC] =     ((uint32_t) (proc_memregion.base +1));
	proc_list.list[proc_list.count].sp[OFFSET_APSR] =   ((uint32_t) INITIAL_APSR);
	
	//Increment counter in list
	proc_list.count++;
	
	//Start ticking on first process (idle thread is process/thread 1)
	if( proc_list.count == 2 ){
		hal_cpu_set_psp( proc_list.list[0].sp );						//or else the first tick fails
		hal_cpu_systimer_start( TICK_FREQ, tick_callback );
	}
	
	return SCHEDULER_PROCESS_CREATE_SUCCESS;	
}

/*
*	Scheduler Thread Create
*
*	Creates a Thread 
*
*   It's really a process, but processes are so simple, that there's 
*   no  difference between a process and a thread.
*
*/
uint32_t scheduler_thread_create( uint8_t* thread_code, uint8_t* name, uint32_t stack_sz ){
	
	//Set process info
	proc_list.list[proc_list.count].name = name;
	proc_list.list[proc_list.count].state = ProcessStateReady;
	
	//Allocate space for "fake" context
	stack_alloc( CONTEXT_SIZE );
	
	//Allocate space for remaining stack
	proc_list.list[proc_list.count].sp = stack_top();       //set SP
	stack_alloc( stack_sz - CONTEXT_SIZE );					//make space
	
	//Insert "fake" context
	proc_list.list[proc_list.count].sp[OFFSET_LR] =     ((uint32_t) (process_thread_delete +1));
	proc_list.list[proc_list.count].sp[OFFSET_PC] =     ((uint32_t) (thread_code +1));
	proc_list.list[proc_list.count].sp[OFFSET_APSR] =   ((uint32_t) INITIAL_APSR);
	
	//Increment counter in list
	proc_list.count++;
		
	return SCHEDULER_PROCESS_CREATE_SUCCESS;
}

void scheduler_process_current_stop(void){
	//changes thread state to dead
	//(so it's not scheduled anymore)
	active_proc->state =  ProcessStateDead;
	
	//context switches
	hal_cpu_lowpty_softint_trigger();
}