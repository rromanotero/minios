/*
 * led.c
 * 
 * LEDs
 */ 

#include "../minilib/pio.h"
#include "led.h"

void led_write( tLedNum led_num, bool state ){
	tPioPin led_pin;	
	
	//get pin and set direction
	switch(led_num){
		case Led0:	pio_create_pin( &led_pin, PioC, 23 ); break; 
		case Led1:	pio_create_pin( &led_pin, PioC, 20 ); break; 
		case Led2:	pio_create_pin( &led_pin, PioA, 16 ); break; 
		case Led3:	pio_create_pin( &led_pin, PioC, 22 ); break; 
		case Led4:	pio_create_pin( &led_pin, PioC, 19 ); break; 
	}
	
	//set direction
	pio_set_pin_dir( &led_pin, PioPinDirOutput );
	
	//write
	pio_write_pin( &led_pin, state );
}


bool led_read( tLedNum led_num ){
	tPioPin led_pin;
	
	//get pin
	switch(led_num){
		case Led0:	pio_create_pin( &led_pin, PioC, 23 ); break;
		case Led1:	pio_create_pin( &led_pin, PioC, 20 ); break;
		case Led2:	pio_create_pin( &led_pin, PioA, 16 ); break;
		case Led3:	pio_create_pin( &led_pin, PioC, 22 ); break;
		case Led4:	pio_create_pin( &led_pin, PioC, 19 ); break;
	}
	
	return pio_read_pin( &led_pin );
}