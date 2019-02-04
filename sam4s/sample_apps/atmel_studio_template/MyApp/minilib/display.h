/*
 * display.h
 * 
 * Display
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint-gcc.h>	

void display_printf( const uint8_t* format, ... );
void display_puts( const uint8_t* );
void display_putc( uint8_t );
void display_cls( void );
void display_gotoxy( uint32_t, uint32_t );
uint32_t display_numlines( void );
uint32_t display_currline( void );

#endif /* DISPLAY_H_ */