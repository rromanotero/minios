/*
 * mtimer.h
 * 
 * Milli-second timer
 *
 */ 

#ifndef MTIMER_H_
#define MTIMER_H_

#include <stdint-gcc.h>	

void mtimer_start_poll( uint32_t );
//void mtimer_start_ev( uint32_t ); NOT SUPPORTED YET
void mtimer_stop( void );
uint32_t mtimer_read( void );

#endif /* MTIMER_H_ */