/*
 * scheduling_policy.c
 *
 */ 

#include "system.h"
#include "scheduler.h"

uint32_t process_mark = 0;

/*
* NOTE: This needs a better name. Scheduling Policy sounds so
*       INSURANCEsy
*
*       I believe this is round robin! It jsut looks different thatn what
*		it appears on textsbooks.
*/
tMiniProcess* scheduling_policy_next( tMiniProcess* active_proc, tProcessList* proc_list  ){
	
	//Increment process mark
	//(skip dead processes)
	uint32_t count=0;
	do{
		count++;
		process_mark = (process_mark + 1) % proc_list->count;
	}while( proc_list->list[process_mark].state == ProcessStateDead );
	
	
	//Return process at process mark
	return &(proc_list->list[process_mark]);
}
