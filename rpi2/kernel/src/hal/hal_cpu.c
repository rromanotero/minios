#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "hal.h"
#include "../drivers/smartstart/rpi-SmartStart.h"

//IRQ-related defintions
void (*irqs_callback)(void);

//SVC-related defintions
void (*svc_callback)(void);


/**
*	Interrupts Start
*
*	Starts IRQs and registers a callback function
*
*/
void hal_cpu_irqs_start( void(*callback)(void) ){

	//Enable different IRQs
	//idea 1 //IrqControlRegisters->(EnableBasicIrqReg)EnableBasicIRQs->Enable_Timer_IRQ
	//SYSTEMTIMER->Control->TimerIrqEnable = 1;
	irqs_callback = callback; //IRQ Handler definition is in hal_cpu_asm.s
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
