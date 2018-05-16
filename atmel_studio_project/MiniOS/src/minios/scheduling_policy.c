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
	process_mark = (process_mark + 1) % proc_list->count;
	
	//Return process at process mark
	return &(proc_list->list[process_mark]);
}
