/**
 * @file	syscalls.c
 * @author  
 * @version 
 *
 * @brief System Calls Interface 
 *
 */

#include "system.h"
#include "HAL/hal.h"
#include "syscalls.h"
#include "scheduler.h"

void syscalls_entry_point(void);

/**
*	Syscalls Init
*
*	Initializes the system calls interface
*
*/
void syscalls_init(void){
	//Starts SVCs (registers SVC callback)
	hal_cpu_svc_start( syscalls_entry_point );
}

/**
*	Syscalls Entry Point
*
*	This is the execution entry point of a system call. That is, where execution
*	goes on SVC call.
*
*   Important: We assume the processor is always in user mode when executing SVC.
*	If the processor is not this fails! Should one want to extend it to support 
*	system calls made while in handler mode, the following is necessary:
*	   1. Figure out the state of the CPU before exception entry (thread or handler)
*         by inspecting EXC_RETURN
*	   2. Based on that extract the right SP (PSP or MSP)
*	   3. The rest is the same...
*
*	There' no reason to make a syscall from the kernel though, so it's all good.
*/
void syscalls_entry_point(void){
	
	uint32_t sp = hal_cpu_get_psp();
	
	//Get syscall number from SVC instruction in program memory
	//- - - - - - -- - - - - - - - - - - - - - - - - - - - - - -
	//To find its address we look at the return address stacked on
	//exception entry (offset 6 from SP).
	//That stacked return address is the address of the instruction to
	//be executed on exception return (typically bx lr).
	//We know bx lr is a 16-bit instruction. So to get the SVC intruction
	//we just read 2 bytes before the specified address.
	uint32_t svc_number = ((uint16_t*) ((uint32_t*)sp)[6])[-1];
	
	//extract the number from the read instruction
	svc_number &= 0x00FF;
	
	//get arguments
	void* arg0 = ((uint32_t*)sp)[0];
	void* arg1 = ((uint32_t*)sp)[1];
	void* arg2 = ((uint32_t*)sp)[2];
	void* arg3 = ((uint32_t*)sp)[3];
	
	//attend syscall
	switch(svc_number){		
		//Button
		//case SVCButtonStartEv:		/* Not supported yet */										break;
		case SVCButtonStartallPoll:	hal_io_button_startall_poll();										break;	
		case SVCButtonRead:			*((tButtonState*)arg1) = hal_io_button_read( (tButtonNum)arg0 );	break;	
		
		//Clock
		case SVCClockWrite:			hal_io_clock_write( (tTime*)arg0 );								break;
		case SVCClockRead:			hal_io_clock_read( (tTime*)arg0  );								break;
		
		//Serial
		//case SVCSerialStartEv:		/*Not supported yet  */										break;
		case SVCSerialStartPoll:	hal_io_serial_start_poll( (tSerialId)arg0, (uint32_t)arg1 );		break;
		case SVCSerialPutc:			hal_io_serial_putc( (tSerialId)arg0, (uint8_t)arg1 );				break;
		case SVCSerialGetc:			*((uint32_t*)arg1) = hal_io_serial_getc( (tSerialId)arg0 );		break;
		
		//Sensor
		//case SVCSensorStartEv:		/* Not supported yet */										break;
		case SVCSensorStartPoll:	hal_io_sensor_start_poll( (tSensorId)arg0 );						break;
		case SVCSensorRead:			*((uint32_t*)arg1) = hal_io_sensor_read( (tSensorId)arg0 );		break;
		
		//Display		
		case SVCDisplayPutc:		hal_io_display_putc( (uint8_t)arg0 );								break;
		case SVCDisplayCls:			hal_io_display_cls();												break;
		case SVCDisplayGotoxy:		hal_io_display_gotoxy( (uint32_t)arg0, (uint32_t)arg1 );			break;
		case SVCDisplayNumLines:	*((uint32_t*)arg0) = hal_io_display_numlines();					break;
		case SVCDisplayCurrLine:	*((uint32_t*)arg0) = hal_io_display_currline();					break;
		
		//Millisecond Timer
		//case SVCMtimerStartEv:	/*Not supported yet*/											break;
		case SVCMtimerStartPoll:	hal_io_mtimer_start( (uint32_t)arg0  );							break;
		case SVCMtimerStop:			hal_io_mtimer_stop();											break;
		case SVCMtimerRead:			*((uint32_t*)arg0) = hal_io_mtimer_read( );						break;

		//System
		case SVCSysinfo:
		/* Not available yet. Here the system should strcpy the system name and version
		   into a userland buffer. System name and version are available from system.h
		   
		   Also add later a HdwMemInfo, HdwCPuInfo, etc, for info about the hdw platform.
		   This should be from somewhere in the HAL
		    */
		break;

		//Scheduler	
		case SVCProcessCreate:		scheduler_process_create( (uint8_t*)arg0, (uint8_t*)arg1, (uint32_t*)arg2 );	break;
		case SVCThreadCreate:		scheduler_thread_create( (uint8_t*)arg0, (uint8_t*)arg1, (uint32_t)arg2 );		break;
		case SVCProcessStop:		scheduler_process_current_stop();												break;
		
		//Parallel IO
		case SVCPIOCreatePin:		hal_io_pio_create_pin( (tPioPin*)arg0, (tPio)arg1, (uint32_t)arg2 );			break;
		case SVCPIOWritePin:		hal_io_pio_write_pin( (tPioPin*)arg0, (bool)arg1 );								break;
		case SVCPIOReadPin:			*((uint32_t*)arg1) = hal_io_pio_read_pin( (tPioPin*)arg0 );						break;
		case SVCPIOSetPinDir:		hal_io_pio_set_pin_dir( (tPioPin*)arg0, (tPioPinDir)arg1 );						break;
		
		//PWM
		case SVCPWMChannelStart:	hal_io_pwm_channel_start( (tPwmChannel*)arg0 );									break;
		case SVCPWMChannelWrite:	hal_io_pwm_channel_write( (tPwmChannel*)arg0 );									break;
		case SVCPWMChannelStop:		hal_io_pwm_channel_stop( (tPwmChannel*)arg0 );									break;
		
		//ADC
		case SVCADCChannelStart:	hal_io_adc_channel_start();														break;
		case SVCADCChannelEnable:	hal_io_adc_channel_enable( (tAdcChannel*) arg0);								break;
		case SVCADCChannelDisable:	hal_io_adc_channel_disable( (tAdcChannel*) arg0);								break;
		case SVCADCChannelStatus:	hal_io_adc_channel_status( (tAdcChannel*) arg0);								break;
		case SVCADCChannelRead:		*((uint32_t*)arg1) = hal_io_adc_channel_read( (tAdcChannel*) arg0);				break;
		
		//Error
		default:																									break;
	}
}