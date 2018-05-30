/**
 * @file	syscalls.h
 * @author  
 * @version 
 *
 * @brief System Calls Interface header file
 *
 */

#ifndef SYSCALLS_H_
#define SYSCALLS_H_

void syscalls_init(void);

/**
*	System call numbers
*/
enum{
	//LED
	//FIX THIS
	//SVCLedWrite				= 0,
	//SVCLedRead				= 1,

	//Button
	SVCButtonStartallEv		= 2,
	SVCButtonStartallPoll	= 3,
	SVCButtonRead			= 4,

	//Clock
	SVCClockRead			= 5,
	SVCClockWrite			= 6,
	
	//Serial
	SVCSerialStartEv		= 7,
	SVCSerialStartPoll		= 8,
	SVCSerialPutc			= 9,
	SVCSerialGetc			= 10,
	
	//Sensors
	SVCSensorStartEv		= 11,
	SVCSensorStartPoll		= 12,
	SVCSensorRead			= 13,

	//Display
	SVCDisplayCls			= 14,
	SVCDisplayPutc			= 15,
	SVCDisplayGotoxy		= 16,
	SVCDisplayNumLines		= 17,
	SVCDisplayCurrLine		= 18,

	//Millisecond timer
	SVCMtimerStartEv		= 19,
	SVCMtimerStartPoll		= 20,
	SVCMtimerStop			= 21,
	SVCMtimerRead			= 22,

	//System
	SVCSysinfo				= 23,
	
	//Scheduler
	SVCProcessCreate		= 27,
	SVCThreadCreate			= 28,
	SVCProcessStop			= 29,
	
	//Parallel IO
	SVCPIOCreatePin			= 31,
	SVCPIOWritePin			= 32,
	SVCPIOReadPin			= 33,
	SVCPIOSetPinDir			= 34,
	
	//PWM
	SVCPWMChannelStart		= 38,
	SVCPWMChannelWrite		= 39,
	SVCPWMChannelStop		= 40,
	
	//ADC
	SVCADCChannelStart		= 45,
	SVCADCChannelEnable		= 46,
	SVCADCChannelDisable	= 47,
	SVCADCChannelStatus		= 48,
	SVCADCChannelRead		= 49
	
};



#endif /* SYSCALLS_H_ */