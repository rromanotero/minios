#include <stdint-gcc.h>
#include <stdbool.h>
#include "minilib/pio.h"
#include "minilib/pwm.h"
#include "minilib/display.h"

#define MINIOSAPP __attribute__ ((section(".entry_point"))) uint32_t
#define STACKINFO __attribute__ ((section(".stack_info"))) uint32_t

STACKINFO stack_sz = 512;

void led_dimmer(void);

MINIOSAPP main(void){

	display_printf( "Watch LED3" );
		
	thread_create( led_dimmer, "led dimmer thread", 512 );

	while(true);
}

void led_dimmer(){
	
	//Set ups LED pin
	tPioPin	led3;
	pio_create_pin( &led3, PioC, 22 );
	pio_set_pin_dir( &led3, PioPinDirOutput );

	//Set PWM Channels on the same pin
	tPwmChannel channel_led3;
	channel_led3.io_pin = &led3;
	
	//Start PWM channel with a 20ms period and 0% duty cycle
	channel_led3.period = 50;			//5ms period
	channel_led3.duty_cycle = 1;		//1% Duty Cycle
	pwm_channel_start( &channel_led3 );		

	
	while(true){		
		//Increase Duty Cycle by 1
		//(The LED will dim because PWM is inverted)
		channel_led3.duty_cycle = (channel_led3.duty_cycle + 1 ) % 90;
		pwm_channel_write( &channel_led3 );
		
		//delay
		for( volatile uint32_t i =0; i<500000; i++ );	
	}
}
