/*
 * serial.h
 *
 * Serial Communication
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint-gcc.h>

void serial_putc( uint8_t );
uint8_t serial_getc( void );


#endif /* SERIAL_H_ */
