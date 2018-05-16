/*
 * serial.h
 * 
 * Serial Communication
 */ 

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint-gcc.h>	

typedef uint32_t tSerialId;						
enum tSerialId	{ /*SerialA = 0,*/ SerialB = 1 }; //Serial A is reserved for the console

void serial_start_poll( tSerialId, uint32_t );
//void hal_serial_start_ev( tSerialId, uint32_t, void(*)(uint32_t) ); //Not supported
void serial_printf( tSerialId, const uint8_t* format, ... );
void serial_puts( tSerialId, const uint8_t* );
void serial_putc( tSerialId, uint8_t );
uint8_t serial_getc( tSerialId );


#endif /* SERIAL_H_ */