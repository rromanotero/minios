#include <stdint-gcc.h>
#include <stdbool.h>
#include "minilib/led.h"
#include "minilib/display.h"
#include "minilib/clock.h"

#define MINIOSAPP __attribute__ ((section(".entry_point"))) uint32_t
#define STACKINFO __attribute__ ((section(".stack_info"))) uint32_t

extern uint32_t _estack;
STACKINFO sp  = &_estack;

MINIOSAPP main(void){	
	
	tTime clock;
 	
	uint8_t* day_to_month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", 
								"Aug", "Sep", "Oct", "Nov", "Dec" };
	
	while( true ){
		clock_read( &clock );
		display_printf( " %s %d %d,  \n", day_to_month[clock.month-1], clock.day, clock.year );
		display_printf( " %d : %d : %d  \n", clock.hours, clock.minutes, clock.seconds  );
		display_printf( "\n" );
		display_printf( "\n" );
	}

}
