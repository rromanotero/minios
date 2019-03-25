/**
 * @file	syscalls.c
 * @author
 * @version
 *
 * @brief System Calls Interface
 *
 */
 #include <stddef.h>
 #include <stdbool.h>
 #include <stdint.h>
 #include <stdio.h>
 #include "kernel.h"   //IO Enabling  Defines are here

#include "hal/hal.h"
#include "syscalls.h"


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

	// -- Get arguments and Return address in R7--
	//   It has to be placed right here, before r0-r3 get messed up
	uint32_t arg0, arg1, arg2, arg3, ret_address;

	asm volatile (	"mov %[arg0], r0 \n"
									"mov %[arg1], r1 \n"
									"mov %[arg2], r2 \n"
									"mov %[arg3], r3 \n"
									"mov %[ret_address], r14 \n"
              		: [arg0] "=r" (arg0), // output
										[arg1] "=r" (arg1),
										[arg2] "=r" (arg2),
										[arg3] "=r" (arg3),
										[ret_address] "=r" (ret_address)
              		: // no input
              		: // Clobber list
              	);

	// -- Read the SVC number from (Ret Address - 1) --

	//Extract the supervisor call #
	uint32_t svc_number = ((uint32_t*)ret_address)[-1];
	svc_number &= 0xFF;

	//attend syscall
	switch(svc_number){
		//Serial
#ifdef SERIAL_PRESENT
		case SVCSerialPutc:			hal_io_serial_putc( SerialA, (uint8_t)arg0 );				         break;
		case SVCSerialGetc:			*((uint32_t*)arg1) = hal_io_serial_getc( SerialA );		       break;
#endif
		//Display
#ifdef VIDEO_PRESENT
		case SVCDisplayPutc:		hal_io_video_putc( (uint8_t)arg0, 2, VIDEO_COLOR_WHITE );		 break;
#endif
		//Error
		default:																								                             break;
	}

}
