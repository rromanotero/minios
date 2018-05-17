/**
 * @file	hal_pwm.c
 * @author  
 * @version 
 *
 * @brief HAL for PWM
 *
 */

#include <asf.h>
#include "hal.h"

/**
*	Initialize PWM channel and pin
*
*/
void hal_pwm_init_channel(tPwmPin* pwm, tPioPin* pio_pin, tPwmType pin_type, tPwmChan channel, uint32_t pin_number){
	
	pwm->pin_number = pin_number;
	pwm->pin_channel = channel;
	pwm->pwm_type = pin_type;
	pwm->internal_rep = pio_pin->internal_rep;
	
	//PWM Clock options
	pwm_clock_t pwm_clock_opts = {
		//100Khz clock frequency = .01 ms steps
		.ul_clka = 100000, 
		.ul_clkb = 0,
		.ul_mck =  sysclk_get_main_hz() 
	};
	//PWM options
	pwm->pwm_channel.ul_prescaler = PWM_CMR_CPRE_CLKA;
	// period = 150 gives us 1.5ms maximum period
	pwm->pwm_channel.ul_period = 150;
	// 10% duty cycle for duty = 7
	pwm->pwm_channel.ul_duty = 7; 
	pwm->pwm_channel.polarity = PWM_HIGH;
	switch (pwm->pin_channel)
	{
		case PwmChan0:
		pwm->pwm_channel.channel = PWM_CHANNEL_0;
		break;
		case PwmChan1:
		pwm->pwm_channel.channel = PWM_CHANNEL_1;
		break;
		case PwmChan2:
		pwm->pwm_channel.channel = PWM_CHANNEL_2;
		break;
		case PwmChan3: 
		pwm->pwm_channel.channel = PWM_CHANNEL_3;
		break;
		default :
		printf(" Invalid channel ");
	}
	//configure IO pins
	switch (pwm->pwm_type)
	{
		case PeriphA:
		pio_configure_pin( pwm->internal_rep, PIO_TYPE_PIO_PERIPH_A );
		break;
		case PeriphB:
		pio_configure_pin( pwm->internal_rep, PIO_TYPE_PIO_PERIPH_B );
		break;
		case PeriphC:
		pio_configure_pin( pwm->internal_rep, PIO_TYPE_PIO_PERIPH_C );
		break;
		case PeriphD:
		pio_configure_pin( pwm->internal_rep, PIO_TYPE_PIO_PERIPH_D );
		break;
		default :
		printf(" Invalid type ");
	}
	//Enable PWM clock
	pmc_enable_periph_clk(ID_PWM);
	//disable temporarily
	hal_pwm_stop(pwm);
	
	pwm_init( PWM, &pwm_clock_opts );
	pwm_channel_init( PWM, &pwm->pwm_channel );
}

/**
*
*	Enable PWM
*/
void hal_pwm_enable(tPwmPin* pwm_pin){
	switch (pwm_pin->pin_channel)
	{
		case PwmChan0:
		pwm_channel_enable(PWM, PWM_CHANNEL_0);
		break;
		case PwmChan1:
		pwm_channel_enable(PWM, PWM_CHANNEL_1);
		break;
		case PwmChan2:
		pwm_channel_enable(PWM, PWM_CHANNEL_2);
		break;
		case PwmChan3:
		pwm_channel_enable(PWM, PWM_CHANNEL_3);
		break;
		default :
		printf(" Invalid channel ");
	}
}

/**
*
*	Disable PWM
*/
void hal_pwm_stop(tPwmPin* pwm_pin){
	switch (pwm_pin->pin_channel)
	{
		case PwmChan0:
		pwm_channel_disable(PWM, PWM_CHANNEL_0);
		break;
		case PwmChan1:
		pwm_channel_disable(PWM, PWM_CHANNEL_1);
		break;
		case PwmChan2:
		pwm_channel_disable(PWM, PWM_CHANNEL_2);
		break;
		case PwmChan3:
		pwm_channel_disable(PWM, PWM_CHANNEL_3);
		break;
		default :
		printf(" Invalid channel ");
	}
}

/**
*
*	Update PWM period
*/
void hal_pwm_update_period(tPwmPin* pwm, uint32_t period){
	pwm_channel_update_period( PWM, &pwm->pwm_channel, period );
}

/**
*
*	Update PWM duty cycle
*/
void hal_pwm_update_duty(tPwmPin* pwm, uint32_t duty){
	pwm_channel_update_duty( PWM, &pwm->pwm_channel, duty );
}