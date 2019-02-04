/*
 * clock.h
 * 
 * Clock
 */ 

#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint-gcc.h>	

typedef struct{
	uint32_t seconds; 
	uint32_t minutes; 
	uint32_t hours;   
	uint32_t day;     
	uint32_t month;   
	uint32_t year;    
}tTime;

void clock_write( tTime* );
void clock_read( tTime* );

#endif /* CLOCK_H_ */