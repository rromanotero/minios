/*
 * hal_pio.h
 *
 * Created: 2018-05-15 6:16:29 PM
 *  Author: Conan
 */ 


#ifndef HAL_PIO_H_
#define HAL_PIO_H_

typedef enum {
	PIO_A,
	PIO_B,
	PIO_C,
	PIO_D
} pio_port;

#define HIGH	0
#define LOW		1

ioport_pin_t hal_create_pio_pin(pio_port pp, int pin_num);
void hal_set_pio_level(uint32_t pio_pin, uint32_t level);
void hal_set_pio_high(uint32_t pio_pin);
void hal_set_pio_low(uint32_t pio_pin);
bool hal_read_pio_state(uint32_t pio_pin);

#endif /* HAL_PIO_H_ */