#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "hal.h"
#include "../irq.h"
#include "../drivers/per_core_timer/per_core_timer.h"
#include <stdio.h>

//Systick-related defintions
void (*systick_callback)(void);
uint32_t ms_to_cntv_val( uint32_t );

//IRQ-related defintions
void (*irq_callback)(void);

//SVC-related defintions
void (*svc_callback)(void);


/**
*	SystemTimer Start
*
*	Starts the System Timer
*
*  NOTE:
*		  MiniOS System Timer is plugged to The PI's Generic TIMER
*			This is so we can test on QEMU (systimer is not implemented in QEMU).
*
*	@param tick_freq_in_ms the tick frequency in milliseconds
*	@param callback function to be called when a tick occurs
*/
void hal_cpu_systimer_start(uint32_t tick_freq_in_ms, void(*callback)(void)){

	//Set up Callback
	irq_register_callback( IrqSystemTimer, hal_cpu_systimer_is_pending, callback );

	//Set up counter value
	per_core_timer_write_cntv_tval( ms_to_cntv_val(tick_freq_in_ms) );

	//begin generic timer (this MUST be the LAST STEP)
	per_core_timer_enable();
}

uint32_t ms_to_cntv_val( uint32_t ms ){
  return per_core_timer_read_cntfrq()*(ms/1000);
}

/**
*	SystemTimer Stop
*
*	Stops the system timer
*
*/
void hal_cpu_systimer_stop(void){

}

/**
*	SystemTimer reestart
*
*	Once started, this function can be used to re-estart the system timer
*	with the same configuration.
*
*/
void hal_cpu_systimer_reestart( uint32_t tick_freq_in_ms ){
	per_core_timer_write_cntv_tval( ms_to_cntv_val(tick_freq_in_ms) );
}

/**
*	SystemTimer reestart
*
*	Once started, this function can be used to re-estart the system timer
*	with the same configuration.
*
*/
bool hal_cpu_systimer_is_pending( void ){
	return per_core_timer_read_core0timer_pending() & 0x08;
}

/**
*	Interrupts Start
*
*	Starts all IRQs and registers a callback function
*
*/
void hal_cpu_irq_start( void(*callback)(void) ){

	irq_callback = callback; //IRQ Handler definition is in hal_cpu_asm.s
}

/**
*	SVC Start
*
*	Starts SVC calls and registers a callback function. The callback
*	execution of an SVC instruction
*
*	@param callback the function that gets called on supervisor calls
*/
void hal_cpu_svc_start( void(*callback)(void) ){
	svc_callback = callback; //SVC Handler definition is in hal_cpu_asm.s
}
