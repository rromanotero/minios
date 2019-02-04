/**
 * @file	console.c
 * @author  
 * @version 
 *
 * @brief Header file for the commands module
 *
 */


#ifndef COMMANDS_H_
#define COMMANDS_H_

void commands_ls( uint8_t*, uint32_t );
void commands_cat( uint8_t*, uint32_t );
void commands_date( uint8_t*, uint32_t );
void commands_sysinfo( uint8_t*, uint32_t );

#endif /* COMMANDS_H_ */