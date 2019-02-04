/**
 * @file	hal_memreg.c
 * @author  
 * @version 
 *
 * @brief Memory Regions part of the Hardware Abstraction Layer. 
 *
 */
#include "hal.h"
#include "../system.h"

//System Memory Region
#define MEM_REGION_SYS_BASEPTR		(uint8_t*)0x20000000		
#define MEM_REGION_SYS_SIZE			SYS_SYSTEM_MEM_AVAILABLE	

//App Memory Region
//( Starts right after System + 4 bytes (for SP ))
#define MEM_REGION_APP_BASEPTR		(uint8_t*)( MEM_REGION_SYS_BASEPTR + MEM_REGION_SYS_SIZE + 4 )		
#define MEM_REGION_APP_SIZE			SYS_APP_MAX_SIZE-4			

//Stack Memory Region
#define MEM_REGION_STACK_BASEPTR	( MEM_REGION_APP_BASEPTR + MEM_REGION_APP_SIZE )
#define MEM_REGION_STACK_SIZE		SYS_USER_STACK_MAX_SIZE		

//Linker script variables for System Stack	
extern uint32_t _estack;				//end of stack
extern uint32_t __stack_size__;			//stack's size

/**
*	HAL Memory Regions Init
*
*	Initializes Memory. This function must be called after
*	HAL CPU Init. That is: hal_cpu_init(); hal_mem_init();...
*
*/
void hal_memreg_init(void){
	//nothing to initialize...
	//for consistency...
	//and compatibility if future versions require initialization
}

/**
*	HAL Memory Region Read
*
*	Reads information related to the specified memory region.
*
*	@param memid	the specified memory region
*	@param memreg	a pointer to the tMemRegion variable to be populated
*/
void hal_memreg_read( tMemRegionId memid, tMemRegion* memreg ){
	if( memreg == 0 ) return; //Error (null ptr)
	
	memreg->id = memid;
	
	switch( memid ){
		case MemRegSystem:
			memreg->base = MEM_REGION_SYS_BASEPTR;
			memreg->size  = MEM_REGION_SYS_SIZE;
			break;
		case MemRegApp:
			memreg->base = MEM_REGION_APP_BASEPTR;
			memreg->size  = MEM_REGION_APP_SIZE;
			break;
		case MemRegSystemStack:
			memreg->base = &_estack;				//base = stack's end address
			memreg->size = &__stack_size__;			
			break;
		case MemRegUserStack:
			memreg->base = MEM_REGION_STACK_BASEPTR;				//base = stack's end address
			memreg->size = MEM_REGION_STACK_SIZE;
			break;
		default:	
			//Error		  
			break;
	}
	
	return 0;
}
