#include "system.h"
#include "hal/hal.h"
#include "loader.h"
#include "utils/stack.h"
#include "scheduler.h"

#define TICK_FREQ		3
#define CONTEXT_SIZE    16
#define INITIAL_APSR    (1 << 24) //Bit 24 is the Thumb bit
#define OFFSET_PC       14
#define OFFSET_APSR     15

__attribute__((naked)) static void tick_callback(void);

typedef struct{
	uint8_t* name;
	uint32_t* sp;
	tProcessState state;
}MiniProcess;

static MiniProcess proc[1];			// processes
static MiniProcess* wait_list[1];	//dummy waiting list
static MiniProcess* active_proc;	//The active process
static uint32_t proc_count = 0;

//A null process (used to mark the lack of an active process)
static MiniProcess null_proc = { 
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
	
	//Active process is the null process
	active_proc = &null_proc;
}

/*
*	The infamous tick callback
*
*	Context switch takes place here.
*/
__attribute__((naked)) static void tick_callback(void){
	
	//save software context
	hal_cpu_save_context();
	
	//Is there an active process?
	if( active_proc->state != ProcessStateNull ){
		//save SP
		active_proc->sp = hal_cpu_get_psp();
		
		//place active process in dummy waiting list
		wait_list[0] = active_proc;
	}
	
	//get next active process from dummy waiting list
	active_proc = wait_list[0];
	
	//restore SP
	hal_cpu_set_psp( active_proc->sp );
	
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
	proc[0].name = name;
	proc[0].state = ProcessStateReady;
	
	//Allocate space for "fake" context
	stack_alloc( CONTEXT_SIZE );
	
	//Allocate space for remaining stack
	proc[0].sp = stack_top();                //set SP
	stack_alloc( stack_sz - CONTEXT_SIZE );  //make space 
	
	//Insert "fake" context
	proc[0].sp[OFFSET_PC] =     ((uint32_t) (proc_memregion.base +1));
	proc[0].sp[OFFSET_APSR] =   ((uint32_t) INITIAL_APSR);
	
	//Add new process to dummy wait list
	wait_list[0] = &proc[0];
	proc_count++;
	
	//Start ticking on first process
	if( proc_count == 1 ){
		hal_cpu_set_psp( proc[0].sp );						//or else the first tick fails
		hal_cpu_systimer_start( TICK_FREQ, tick_callback );
	}
	
	return SCHEDULER_PROCESS_CREATE_SUCCESS;	
}