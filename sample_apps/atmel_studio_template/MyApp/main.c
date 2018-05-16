#include <stdint-gcc.h>
#include <stdbool.h>
#include "drivers/led.h"

#define MINIOSAPP __attribute__ ((section(".entry_point"))) uint32_t
#define STACKINFO __attribute__ ((section(".stack_info"))) uint32_t

STACKINFO stack_sz = 512;
void small_delay(void);	//Don't remove this prototype. or small_delay gets optimized away

MINIOSAPP main(void){
	
	for(;;){
		led_write( Led0, !led_read(Led0) );
		led_write( Led1, !led_read(Led1) );
		led_write( Led2, !led_read(Led2) );
		led_write( Led3, !led_read(Led3) );
		led_write( Led4, !led_read(Led4) );
		small_delay();
	}
	
	return 0;
}

void small_delay(){
	//Don't forget volatile! 
	#define NINE_MILLION 9000000
	for( volatile uint32_t i =0; i<NINE_MILLION; i++ );
}
