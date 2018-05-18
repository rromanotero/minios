/*
 * pio.h
 * 
 * Parallel IO
 */ 

#ifndef PIO_H_
#define PIO_H_

#include <stdint-gcc.h>
#include <stdbool.h>

typedef uint32_t tPioPort;
typedef uint32_t tPioPinDir;	/**< Parallel IO Pin Direction */

enum tPioPort		{ PioA = 0, PioB, PioC, PioD } ;
enum tPioPinDir		{ PioPinDirOutput = 0, PioPinDirInput } ;

typedef struct{
	uint32_t		pin_number;
	tPioPort		pio_port;
	uint32_t		internal_rep; /* How the pin is represented internally (this is hardware specific) */
}tPioPin;

void pio_create_pin(tPioPin*, tPioPort, uint32_t);
void pio_write_pin(tPioPin*, bool);
bool pio_read_pin(tPioPin*);
void pio_set_pin_dir(tPioPin*, tPioPinDir);

#endif /* PIO_H_ */