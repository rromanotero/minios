#include <stdint-gcc.h>
#include <stdbool.h>
#include "minilib/button.h"
#include "minilib/display.h"
#include "minilib/mtimer.h"
#include <stdio.h>

#define MINIOSAPP __attribute__ ((section(".entry_point"))) uint32_t
#define STACKINFO __attribute__ ((section(".stack_info"))) uint32_t

extern uint32_t _estack;
STACKINFO sp  = &_estack;

#define NUM_BUTTONS 4

MINIOSAPP main(void){	
	button_startall_poll();
	
	tButtonState state[NUM_BUTTONS]; 
	tButtonState state_prev[NUM_BUTTONS]; 
	uint32_t count[NUM_BUTTONS]; 
	
	//init 
	for( uint32_t i=0; i<NUM_BUTTONS; i++ ){
		state[i] = state_prev[i] = ButtonUnpressed;
		count[i] = 0;
	}
	
	//print presses for each button
	while( true ){
		for( uint32_t i=0; i<NUM_BUTTONS; i++ ){
			//update state
			state_prev[i] = state[i];
			state[i] = button_read( i ); 
		
			//count presses
			if( state[i] == ButtonPressed && state_prev[i] == ButtonUnpressed )
				count[i]++;
			
			//print
			display_printf( "Button %d presses: %d \n", i, count[i] );
		}
	}
}
