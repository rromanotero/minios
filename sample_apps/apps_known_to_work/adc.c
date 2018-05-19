#include <stdint-gcc.h>
#include <stdbool.h>
#include "minilib/pio.h"
#include "minilib/pwm.h"
#include "minilib/display.h"
#include "minilib/adc.h"

#define MINIOSAPP __attribute__ ((section(".entry_point"))) uint32_t
#define STACKINFO __attribute__ ((section(".stack_info"))) uint32_t

STACKINFO stack_sz = 512;
void ir_sensor(void);

MINIOSAPP main(void){

	display_printf( "Use IR Sensor" );
		
	thread_create( ir_sensor, "led dimmer thread", 512 );

	while(true);
}

void ir_sensor(){
	
	// Test led vcc pin, ext1 pin 8
	tPioPin pin1;
	pio_create_pin(&pin1, PioA, 19);
	pio_set_pin_dir(&pin1, PioPinDirOutput);
	pio_write_pin(&pin1, 0);
	
	// Sensor input pin, ext1 pin 4
	tPioPin pin2;
	pio_create_pin(&pin2, PioA, 18);
	pio_set_pin_dir(&pin2, PioPinDirInput);
	
	tAdcChannel adcChan = {
		.adc_channel_number = 1,
		.io_pin = &pin2
	};
	adc_channel_start();
	adc_channel_enable(&adcChan);

	while(true){
		//delay
		for( volatile uint32_t i =0; i<500000; i++ );
		uint32_t value = adc_channel_read(&adcChan);
		if(value < 4095){
			pio_write_pin(&pin1, 0);
		}
		else{
			pio_write_pin(&pin1, 1);
		}
	}
	
	
	

		
			
	
}
