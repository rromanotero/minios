/*
 * pwm.h
 * 
 * Pulse Width Modulation
 */ 

#ifndef PWM_H_
#define PWM_H_

#include <stdint-gcc.h>
#include <stdbool.h>

typedef struct{
	tPioPin* io_pin;
	uint32_t period;
	uint32_t duty_cycle;
}tPwmChannel;

void pwm_channel_start( tPwmChannel* );
void pwm_channel_stop( tPwmChannel* );
void pwm_channel_write( tPwmChannel* );

#endif /* PWM_H_ */