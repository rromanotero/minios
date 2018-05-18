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

static uint32_t pin_to_asf_peripheral( tPioPin* );
static uint32_t pin_to_asf_channel( tPioPin* );

/**
*
*	PWM Start
*
*	Starts the PWM Peripheral
*/
void hal_io_pwm_start( void ){
	
	//PWM Clock options
	pwm_clock_t pwm_clock_opts = {
		.ul_clka = 10000,				//10Khz frequency = .1 ms steps
		.ul_clkb = 0,
		.ul_mck =  sysclk_get_main_hz() //main clock speed is 240Mhz!, not 120mhz
	};
	
	//Enable PWM clock
	pmc_enable_periph_clk(ID_PWM);
	
	//Disables all Channels 
	//(we don't want any PWM channel to start going before they're configure)
	pwm_channel_disable( PWM, PWM_CHANNEL_0 );
	pwm_channel_disable( PWM, PWM_CHANNEL_1 );
	pwm_channel_disable( PWM, PWM_CHANNEL_2 );
	pwm_channel_disable( PWM, PWM_CHANNEL_3 );
	
	//Inits PWM
	pwm_init( PWM, &pwm_clock_opts );
}

/**
*
*	PWM Channel Start
*
*	Starts the PWM Channel associated on a given PIO pin. If there's no PWM 
*	associated with the given pin, nothing happens.
*
*   Period is given in steps of 0.1ms (with a maximum of X millisecond), while duty cycle is given 
*   percentage. Example for creating a square wave of a 1ms period:
*
*	channel.period = 10;
*   channel.duty_dycle = 50;
*   hal_io_pwm_start( &channel );
*
*/
void hal_io_pwm_channel_start( tPwmChannel* channel ){	
	
	//ASF PWM Options for channel
	pwm_channel_t pwm_opts = {
		.ul_prescaler = PWM_CMR_CPRE_CLKA,		
		.ul_period	= channel->period,			
		.ul_duty = channel->duty_cycle,
		.channel = pin_to_asf_channel(channel->io_pin)
	};
	
	//configure IO pin
	pio_configure_pin( channel->io_pin->internal_rep, pin_to_asf_peripheral(channel->io_pin) );	
	
	//Inits channel
	pwm_channel_init( PWM, &pwm_opts );
	
	//disable temporarily
	//pwm_channel_disable( PWM, pin_to_asf_channel(channel->io_pin) );
	
	//Enables channel
	pwm_channel_enable( PWM, pin_to_asf_channel(channel->io_pin) );
}

static uint32_t pin_to_asf_peripheral( tPioPin* pin ){

	uint32_t peripheral = 0;
	
	if(		pin->pio_port == PioA && pin->pin_number == 19 )	peripheral = PIO_TYPE_PIO_PERIPH_B;
	else if( pin->pio_port == PioA && pin->pin_number == 20 )	peripheral = PIO_TYPE_PIO_PERIPH_B; 
	else if( pin->pio_port == PioA && pin->pin_number == 30 )	peripheral = PIO_TYPE_PIO_PERIPH_A; 
	else if( pin->pio_port == PioC && pin->pin_number == 22 )	peripheral = PIO_TYPE_PIO_PERIPH_B;
	else {
		/* Pin has no associated channel with it*/
	}	
		
	return peripheral;
}

static uint32_t pin_to_asf_channel( tPioPin* pin ){

	uint32_t asf_channel = 0;
	
	if(	pin->pio_port == PioA && pin->pin_number == 19 )		asf_channel = PWM_CHANNEL_0;
	else if( pin->pio_port == PioA && pin->pin_number == 20 )	asf_channel = PWM_CHANNEL_1;
	else if( pin->pio_port == PioA && pin->pin_number == 30 )	asf_channel = PWM_CHANNEL_2;
	else if( pin->pio_port == PioC && pin->pin_number == 22 )	asf_channel = PWM_CHANNEL_3;
	else {
		/* Pin has no associated channel with it*/
	}
	
	return asf_channel;
}

/**
*	PWM Stop
*
*	Stops a PWM 
*/
void hal_io_pwm_channel_stop( tPwmChannel* channel ){
	pwm_channel_disable( PWM, pin_to_asf_channel(channel->io_pin) );
}

/**
*	PWM Write 
*	
*	Updates period and duty cycle for a PWM
*/
void hal_io_pwm_channel_write( tPwmChannel* channel ){
	
	//ASF PWM Options for channel
	pwm_channel_t pwm_opts = {
		.ul_prescaler = PWM_CMR_CPRE_CLKA,
		.ul_period	= channel->period,
		.ul_duty = channel->duty_cycle,
		.channel = pin_to_asf_channel(channel->io_pin)
	};
	
	//Update
	pwm_channel_update_period( PWM, &pwm_opts, channel->period );
	pwm_channel_update_duty( PWM, &pwm_opts, channel->duty_cycle );
}
