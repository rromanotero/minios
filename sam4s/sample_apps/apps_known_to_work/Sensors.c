#include <stdint-gcc.h>
#include <stdbool.h>
#include "minilib/threads.h"
#include "minilib/display.h"
#include "drivers/led.h"

#define MINIOSAPP __attribute__ ((section(".entry_point"))) uint32_t
#define STACKINFO __attribute__ ((section(".stack_info"))) uint32_t

STACKINFO stack_sz = 512;
void thread_john_baptiste(void);
void thread_joanna(void);

MINIOSAPP main(void){
	
	thread_create( thread_john_baptiste, "john baptiste", 512 );
	thread_create( thread_joanna, "joanna", 512 );

	for(;;){
		display_printf( "Main                \n" );
		led_write( Led0, !led_read(Led0) );
		
		//delay
		for( volatile uint32_t i =0; i<2000000; i++ );
	}
		
	return 0;
}

void thread_john_baptiste(){
	for(;;){
		display_printf( "John Baptiste\n" );
		led_write( Led1, !led_read(Led1) );
		led_write( Led2, !led_read(Led2) );
		
		//delay
		for( volatile uint32_t i =0; i<1005000; i++ );
	}
}

void thread_joanna(){
	for(;;){
		display_printf( "Joanna           \n" );
		led_write( Led3, !led_read(Led3) );
		led_write( Led4, !led_read(Led4) );
		
		//delay
		for( volatile uint32_t i =0; i<1000000; i++ );
	}
}
