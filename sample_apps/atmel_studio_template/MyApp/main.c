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

	display_printf( "Put Something Near The IR Sensor" );
		
	thread_create( ir_sensor, "led dimmer thread", 512 );

	while(true);
}

void ir_sensor(){
	
	tPioPin pin1;
	pio_create_pin(&pin1, PioA, 19);
	pio_set_pin_dir(&pin1, PioPinDirOutput);
	pio_write_pin(&pin1, 0);
	
	tPioPin pin2;
	pio_create_pin(&pin2, PioC, 30);
	pio_set_pin_dir(&pin2, PioPinDirInput);
	
	tAdcChannel adcChan = {
		.adc_channel_number = 14,
		.io_pin = &pin2
	};
	adc_channel_start();
	adc_channel_enable(&adcChan);

	while(true){
		//delay
		for( volatile uint32_t i =0; i<500000; i++ );
		//chan1.duty_cycle = (chan1.duty_cycle + 1) % 50;
		//hal_io_pwm_channel_write( &chan1 );
		uint32_t value = adc_channel_read(&adcChan);
		if(value > 60){
			pio_write_pin(&pin1, 1);
		}
		else{
			pio_write_pin(&pin1, 0);
		}
	}
	
	
	

		
			
	
}
