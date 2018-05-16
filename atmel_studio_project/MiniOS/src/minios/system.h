/**
 * @file	system.h
 * @author  
 * @version 
 *
 * @brief System Module header file
 *	Note: system.h must be included before any other MiniOS header file as it
 *	has common includes required by other source code.
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint-gcc.h>

void system_init(void);
void system_panic( const uint8_t* );

//
//   ---------   SYSTEM OPTIONS    ---------
// (Configure system by editing this values)
// (can implement config files instead...)
#define SYS_VERSION					"3"
#define SYS_NAME					"MiniOS"
#define SYS_USERNAME_MAX_LENGTH		10
#define SYS_CONF_SERIAL_A_BAUDRATE	115200
#define SYS_CONF_SERIAL_B_BAUDRATE	115200

#define SYS_SYSTEM_MEM_AVAILABLE	81920							//Total SYSTEM mem available is 80Kb of 160 Total
#define SYS_USER_MEM_AVAILABLE		81920							//Total USER mem available is 80Kb of 160 Total

#define SYS_USER_STACK_MAX_SIZE		(SYS_USER_MEM_AVAILABLE/2)		//HALF of Total User Mem Available
#define SYS_APP_MAX_SIZE			(SYS_USER_MEM_AVAILABLE/4)		//QUARTER of Total User Mem Available  
#define SYS_LOADER_BUFFER_MAX_SIZE	(SYS_USER_MEM_AVAILABLE/4)		//QUARTER of Total User Mem Available

#define SYS_APP_DEFAULT_NAME		"App.bin"
#define SYS_PANIC_MSG_MAX_LENGTH	50
#define SYS_CONSOLE_NEWLINE			"\n\r"		//Some serial console like putty use \n\r as newlines,
												//others like Atmel's Terminal use only \n. Choose accordingly.

#define SYS_MAX_NUM_OF_PROCESSES	10								//Max number of processes supported
	
//   ---------   INIT FROM    ---------
// (Comment one or the other)
#define SYS_INIT_FROM_CONSOLE		//Uncomment to enable and init from the console	
//#define SYS_INIT_FROM_DEFAPP		//Uncomment to disable the console and load SYS_APP_DEFAULT_NAME at init


//   ---------   PRESENT IO    ---------
// (Uncommment to enable)
#define SYS_IO_DISPLAY_PRESENT		//Yes, there's a Display plugged in



#endif /* SYSTEM_H_ */