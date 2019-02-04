#include <stdint-gcc.h>
#include <stdbool.h>
#include "minilib/led.h"
#include "minilib/display.h"

#define MINIOSAPP __attribute__ ((section(".entry_point"))) uint32_t
#define STACKINFO __attribute__ ((section(".stack_info"))) uint32_t

extern uint32_t _estack;
STACKINFO sp  = &_estack;

void small_delay( void );					//Don't forget prototypes!

MINIOSAPP main(void){	
	#define COLUMN_SIZE 7					//Apparently a column is not the size of a letter
	#define NUM_COLUMNS 70
	#define NUM_LETTERS ('Z' - 'A' + 1)
	
	uint32_t num_lines = display_numlines();
	uint32_t column = -COLUMN_SIZE;
	tLedState led_st = LedOff;
	uint8_t letter_ptr = 0;
	
	while(true){
		column = column + COLUMN_SIZE; 
		led_st = !led_st;
		
		for( uint32_t line=0; line<num_lines; line++ ){
			//display
			display_gotoxy( line, column );		
			display_putc( 'A' + letter_ptr );			
			led_write( line+1, led_st );

			small_delay();
			
			if( 'A' + letter_ptr < 'Z' ){
				//continue with next letter
				letter_ptr = (letter_ptr + 1) % NUM_LETTERS;			
			}
			else{
				//start again
				display_cls();
				for(uint32_t i=1; i<5; i++)
					led_write( i, LedOff );
				
				column = -COLUMN_SIZE;
				led_st = LedOff;
				letter_ptr = 0;
				
				break;
			}
		}//end for
	}//end while
}

void small_delay(){
	//Don't forget volatile! Q: Why?
	#define THREE_MILLION 3000000
	for( volatile uint32_t i =0; i<THREE_MILLION; i++ );
}