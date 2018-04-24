/**
 * @file	hal_cpu.c
 * @author  
 * @version 
 *
 * @brief CPU part of the Hardware Abstraction Layer 
 *
 */

#include <asf.h>
#include "hal.h"

//SysTick-related definitions
inline static uint32_t ms_to_ticks(uint32_t time_in_ms){ return (sysclk_get_cpu_hz()/1000)*(time_in_ms); }
void (*systick_callback)(void);

//PendSV-related definitions
void (*pendsv_callback)(void);

//Faults-related definitions
void (*fault_app_callback)(void);
void (*fault_system_callback)(void);

//SVC-related defintions
void (*svc_callback)(void);

/**
*	HAL CPU Init
*
*	Initializes the CPU. This function must be called before 
*	HAL IO Init. That is: hal_cpu_init(); hal_io_init(); 
*/
void hal_cpu_init(void){	
	sysclk_init();  //initialize clocks
}


/**
*	Low Priority Software Interrupt Trigger
*
*	Triggers a PendsSV Exception
*/
void hal_cpu_lowpty_softint_trigger(void){
	SCB->ICSR |= (1<<28);
}

/**
*	Low Priority Software Interrupt Register Callback
*
*	Registers a callback function for the PendSV Exception
*
*	@param callback the function that gets called on PendSV exception
*/
void hal_cpu_lowpty_softint_register_callback( void(*callback)(void) ){
	pendsv_callback = callback;
}

/**
*	SystemTimer Start
*
*	Starts the System Timer
*
*	@param tick_freq_in_ms the tick frequency in milliseconds
*	@param callback function to be called when a tick occurs
*/
void hal_cpu_systimer_start(uint32_t tick_freq_in_ms, void(*callback)(void)){
	//set callback first
	systick_callback = callback;
	
	//start timer
	SysTick_Config( ms_to_ticks(tick_freq_in_ms) );
}

/**
*	SystemTimer Stop
*
*	Stops the system timer
*
*/
void hal_cpu_systimer_stop(void){
	SysTick->VAL   = 0;								/* Load the SysTick Counter Value */
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |	/* Disable SysTick IRQ and SysTick Timer */
	SysTick_CTRL_TICKINT_Msk   |
	(0UL << SysTick_CTRL_ENABLE_Pos);
}

/**
*	SystemTimer reestart
*
*	Once started, this function can be used to re-estart the system timer
*	with the same configuration.
*
*/
void hal_cpu_systimer_reestart(void){
	SysTick->VAL   = 0;								// Load the SysTick Counter Value
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |	// Enable SysTick IRQ and SysTick Timer
	SysTick_CTRL_TICKINT_Msk   |
	SysTick_CTRL_ENABLE_Msk;
}

/**
*	Fault Exception Register Callback
*
*	Registers a generic callback function for CPU Fault Exceptions
*
*	@param callback the function that gets called on fault_type exception
*/
void hal_cpu_fault_register_callback( tFaultOrigin faultOrigin, void(*callback)(void)  ){
		switch(faultOrigin){
			case FaultApp:		fault_app_callback = callback;		break;
			case FaultSystem:	fault_system_callback = callback;	break;
			default:			/* Error */							break;			
		}
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

/**
*	HAL CPU Speed in Hz
*
*	Returns the CPU clock speed in Hz.
*	
*	@return CPU clock speed in Hz
*/
uint32_t hal_cpu_speed_in_hz(void){
	volatile int a = sysclk_get_cpu_hz();
	
	return a;
}

/**
*	CPU Delays
*
*	Busy-waiting delay
*
*	@param delay_in_ms delay in milliseconds
*/
void hal_cpu_delay(uint32_t delay_in_ms){
	delay_ms(delay_in_ms);
}