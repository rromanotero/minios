#include <stdint-gcc.h>
#include <stdbool.h>
#include "minilib/pio.h"
#include "minilib/pwm.h"
#include "minilib/display.h"
#include "minilib/adc.h"

#define MINIOSAPP __attribute__ ((section(".entry_point"))) uint32_t
#define STACKINFO __attribute__ ((section(".stack_info"))) uint32_t

STACKINFO stack_sz = 512;

MINIOSAPP main(void){

	display_printf( "Hello from main\n" );
	for( volatile uint32_t i =0; i<500000; i++ );
	
	display_printf( "Returning..." );
	for( volatile uint32_t i =0; i<500000; i++ );
	
	/*
		To see this working, set breakpoints on kernel.
	*/
	
	return 0;
}
