/**
 * @file	hal_io_pio.c
 * @author  
 * @version 
 *
 * @brief HAL for Parallel IO
 *
 */

#include <asf.h>
#include "hal.h"

/**
*	PIO Start
*
*	Starts Parallel IO
*/
void hal_io_pio_start(void){
	// It does not do anything. Just for compliance with 
	// "all HAL IO has a start function".
	//(Maybe in some platforms, PIO needs some initialization)
}

/**
*	PIO Create
*
*	Creates PIO Pin
*/
void hal_io_pio_create_pin(tPioPin* pio_pin, tPio pio_port, uint32_t pin_number){

	pio_pin->pin_number = pin_number;
	pio_pin->pio_port = pio_port;
	
	switch (pio_port)
	{
		case PioA:
			pio_pin->internal_rep = IOPORT_CREATE_PIN(PIOA, pin_number);
			break;
		case PioB:
			pio_pin->internal_rep = IOPORT_CREATE_PIN(PIOB, pin_number);
			break;
		case PioC:
			pio_pin->internal_rep = IOPORT_CREATE_PIN(PIOC, pin_number);
			break;
		default :
			printf(" Invalid value ");
	}
}


/**
*	PIO Write
*
*	Writes a state/level to a PIO pin
*/
void hal_io_pio_write_pin(tPioPin* pio_pin, bool state){
	ioport_set_pin_level(pio_pin->internal_rep, state);
}

/**
*	PIO Read
*
*	Reads a PIO pin state/level
*/
bool hal_io_pio_read_pin(tPioPin* pio_pin){
	bool state = ioport_get_pin_level(pio_pin->internal_rep);
	return state;
}

/**
*	PIO Set Pin Direction
*
*	Sets the direction of a Parallel IO Pin
*/
void hal_io_pio_set_pin_dir(tPioPin* pio_pin, tPioPinDir dir){
	
	switch(dir){
		case PioPinDirOutput:	ioport_set_pin_dir(pio_pin->internal_rep, IOPORT_DIR_OUTPUT); break;	
		case PioPinDirInput:	ioport_set_pin_dir(pio_pin->internal_rep, IOPORT_DIR_INPUT);  break;	
	}
	
}