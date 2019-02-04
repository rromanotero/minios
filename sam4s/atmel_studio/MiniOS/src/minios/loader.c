/**
 * @file	loader.c
 * @author  
 * @version 
 *
 * @brief App loader. Note: only one param (command-line argument) 
 * is supported.
 *	
 */

#include "minios/HAL/hal.h"
#include "minios/system.h"
#include "minios/loader.h"

static void load_app( uint8_t*, uint8_t*, uint32_t );
static uint32_t inline min( uint32_t a, uint32_t b){ return a < b ? a : b; }

static tMemRegion app_memreg;						//Info to where the binary would be loaded to
static uint8_t binary[SYS_LOADER_BUFFER_MAX_SIZE];	//Binary's temporary buffer,  
static uint32_t app_stack_sz;

void loader_init(void){
	//Get App region details
	hal_memreg_read( MemRegApp, &app_memreg );
}

/*
*
*/
uint32_t loader_load_app( uint8_t* app_name, tMemRegion* p_mem_region, uint32_t* p_stack_sz  ){
	
	//validates app file
	uint32_t app_size = hal_nvmem_fat_file_size( app_name );
	
	if( app_size == 0 )									
		return LOADER_NO_APP_FOUND;
	else if ( app_size > SYS_LOADER_BUFFER_MAX_SIZE  || app_size > SYS_APP_MAX_SIZE )	
		return LOADER_APP_TOO_LARGE;
	
	//reads app
	uint32_t bytes_read = hal_nvmem_fat_file_read( app_name, 
												   binary, 
												   min( SYS_LOADER_BUFFER_MAX_SIZE, SYS_APP_MAX_SIZE ) );

	//Extract app's stack size and the app itself from binary
	app_stack_sz = *((uint32_t*)binary);
	uint32_t* app_buffer = (uint32_t*)binary + 1;

	//load app in App memory region
	load_app( app_memreg.base, app_buffer, bytes_read );

	//pass caller a ptr to the loaded mem region and stack size
	*p_mem_region = app_memreg;
	*p_stack_sz = app_stack_sz;
	
	return LOADER_LOAD_SUCCESS;
}

static void load_app( uint8_t* mem, uint8_t* binary, uint32_t size ){
	
	for(uint32_t i = 0; i<size; i++)
		mem[i] = binary[i];

}
