/**
 * @file	hal_io_adc.c
 * @author  
 * @version 
 *
 * @brief ADC part of the Hardware Abstraction Layer 
 *
 */

#include "asf.h"
#include "hal.h"

/**
*
*	ADM Start
*
*	Starts the ADC, but does NOT stipulate any specific ADC channels to enable.
*
*/
void hal_io_adc_channel_start(){
	/* Enable ADC clock. */
	pmc_enable_periph_clk(ID_ADC);
	/* Configure ADC. */
	adc_init(ADC, sysclk_get_cpu_hz(), 1000000, ADC_MR_STARTUP_SUT0);
	adc_configure_trigger(ADC, ADC_TRIG_SW, 1);
	adc_start(ADC);
}

/**
*	ADC Channel Enable
*
*	Begins ADC conversion on given ADC channel
*/
void hal_io_adc_channel_enable(tAdcChannel* adc_chan){
	adc_enable_channel(ADC, adc_chan->adc_channel_number);
}

/**
*	ADC Channel Disable
*
*	Stops ADC conversion on given ADC channel
*/
void hal_io_adc_channel_disable(tAdcChannel* adc_chan){
	adc_disable_channel(ADC, adc_chan->adc_channel_number);
}

/**
*	ADC Channel Status
*
*	Returns 1 if channel is enabled, 0 otherwise
*/
uint32_t hal_io_adc_channel_status(tAdcChannel* adc_chan){
	uint32_t adc_status = (ADC, adc_chan->adc_channel_number);
	return adc_status;
}

/**
*	ADC Channel value
*
*	Returns a value between 0 - 4096
*/
uint32_t hal_io_adc_channel_read(tAdcChannel* adc_chan){
	uint32_t adc_value = adc_get_channel_value(ADC, adc_chan->adc_channel_number);
	return adc_value;
}

