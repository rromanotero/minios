/*
 * adc.h
 *
 *  Analog to Digital Conversion
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint-gcc.h>
#include <stdbool.h>

typedef struct{
	tPioPin* io_pin;
	uint32_t adc_channel_number;
}tAdcChannel;

void adc_channel_start();
void adc_channel_enable(tAdcChannel* adc_chan);
void adc_channel_disable(tAdcChannel* adc_chan);
uint32_t adc_channel_status(tAdcChannel* adc_chan);
uint32_t adc_channel_read(tAdcChannel* adc_chan);



#endif /* ADC_H_ */