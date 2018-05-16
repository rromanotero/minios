/*
 * hal_pio.c
 *
 * Created: 2018-05-15 6:16:13 PM
 *  Author: Conan
 */ 

#include <asf.h>

// Create PIO pins
ioport_pin_t hal_create_pio_pin(pio_port pp, int pin_num){
	ioport_pin_t pin;
	switch (pp)
	{
		case PIO_A:
		pin = IOPORT_CREATE_PIN(PIOA, pin_num);
		break;
		case PIO_B:
		pin = IOPORT_CREATE_PIN(PIOB, pin_num);
		break;
		case PIO_C:
		pin = IOPORT_CREATE_PIN(PIOC, pin_num);
		break;
		case PIO_D:
		pin = IOPORT_CREATE_PIN(PIOD, pin_num);
		break;
		default :
		printf(" Invalid value ");
	}
	return pin;
}

//  read/write pins
void hal_set_pio_level(uint32_t pio_pin, uint32_t level){
	ioport_set_pin_level(pio_pin, level);
}

void hal_set_pio_high(uint32_t pio_pin){
	hal_set_pio_level(pio_pin, HIGH);
}

void hal_set_pio_low(uint32_t pio_pin){
	hal_set_pio_level(pio_pin, LOW);
}

bool hal_read_pio_state(uint32_t pio_pin){
	bool state = ioport_get_pin_level(pio_pin);
	return state;
}