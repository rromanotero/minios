/*
 * led.h
 * 
 * LEDs
 */ 

#ifndef LED_H_
#define LED_H_

#include <stdint-gcc.h>	
#include <stdbool.h>	

typedef uint32_t tLedNum;					
typedef bool tLedState;						
enum tLedState{ LedOff = false, LedOn };
enum tLedNum{ Led0 = 0, Led1, Led2, Led3 };

void led_write( tLedNum , tLedState );
tLedState led_read( tLedNum );

#endif /* LED_H_ */