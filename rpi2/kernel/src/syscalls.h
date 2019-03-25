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

	//Serial
	SVCSerialPutc			= 9,
	SVCSerialGetc			= 10,

	//Display
	SVCDisplayPutc			= 15,

};



#endif /* SYSCALLS_H_ */
