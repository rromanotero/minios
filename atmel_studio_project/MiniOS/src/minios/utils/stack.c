/**
 * @file	stack.c
 * @author  
 * @version 
 *
 * @brief Stack module. This is a generic module to handle allocation of memory in the stack. 
 *		  It assumes a full-descendant stack, and ensures 8-byte alignment on allocation. 
 *		  This module must be ported when porting MiniOS
 *
 *		  Careful with mixing integer arithmetic with pointer arithmetic.
 */
#include "../system.h"
#include <stdint-gcc.h>
#include <stdbool.h>

static void alloc(uint32_t);
static void align_to_eight_byte_boundary(void);
	
static uint32_t* stack;
static bool initialized = false;

void stack_init(uint32_t* address){
	stack = address;
	initialized = true;
}

uint32_t* stack_top(void){
	if( !initialized )
		system_panic( "Reading uninit stack" );
	
	return stack;
}

void stack_alloc(uint32_t elements){
	if( !initialized )
		system_panic( "Allocation on uninit stack" );
	
	alloc(elements);
	align_to_eight_byte_boundary();
}

static void alloc(uint32_t elements){
	return stack = stack - elements;
}


static void align_to_eight_byte_boundary(void){
	uint32_t address = (uint32_t)stack;
	
	while( address % 8 != 0 ){
		address--;
	}
	
	stack = address;
}