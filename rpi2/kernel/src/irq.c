

#include "hal/hal.h"
#include "irq.h"
#include "hal/hal.h"

void irq_entry_point(void);

/**
*	Interrupts Init
*
*	Initializes the interrupts interface
*
*/
void irq_init(void){
	//Starts Interrupts
	hal_cpu_irq_start( irq_entry_point );

	//Enable IRQs
	hal_cpu_irq_enable();
}

bool(*is_pending_i)(void);
void(*callback_i)(void);

void irq_register_callback( IrqType int_type, bool(*is_pending)(void), void(*callback)(void) ){
	is_pending_i = is_pending;
	callback_i = callback;
}



void irq_entry_point(void){

	if ( is_pending_i() ){
			callback_i();
			//set cntv_tval
			//(Needs to be set again on every tick)
			//per_core_timer_write_cntv_tval( ms_to_cntv_val(5) );
			//hal_io_serial_puts( SerialA, "tick" );

	}

}
