/**
 * @file	console.c
 * @author  
 * @version 
 *
 * @brief A console for MiniOS. To add a new supported command follow these steps:
 *			1. Add its name to cmd_names
 *			2. Create a function with the same name in commands.c
 *			3. Add a ptr to that function to cmd_ptrs
 *		  Note: only one param (command-line argument) is supported. 
 *			    e.g. $cat file.txt works fine, but can't pass more arguments.
 *			To add support change: extract_param function, loader module, and commands module. 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minios/HAL/hal.h"
#include "minios/console/console.h"
#include "minios/console/commands.h"
#include "minios/system.h"
#include "minios/loader.h"
#include "minios/scheduler.h"

static void print( uint8_t* );
static void readln( uint8_t* );
static void extract_command( uint8_t*, uint8_t* );
static uint32_t extract_param( uint8_t*, uint8_t* );
static int32_t command_index( uint8_t* );
static bool is_alpha( uint8_t );
static bool is_number( uint8_t );
static void print_welcome_message(void);

#define MAX_COMMAND_LEN			20		//command length (arbitrary)
#define MAX_PARAM_LEN			20		//param length (arbitrary)
#define MAX_LINE_LEN			MAX_COMMAND_LEN + MAX_PARAM_LEN*2 + 1	//one command + 2 params + null char 
#define MAX_NUM_OF_PARAMS		1		//Only 1 param is supported, for now. 

#define DUMMY_USER		"no-user"	//No support for users yet
#define DEFAULT_PATH	"~"			//Initially the system is in the root directory. No support for dirs yet.
#define CONSOLE_SYMBOL	"$"			//The symbol that shows before the prompt
#define INVALID_CMD		-1
#define SENTINEL_CMD	"[[]]"		//some string that won't match any command
									//Don't know what a sentinel is? See https://en.wikipedia.org/wiki/Sentinel_value

//List of command names (Add new commands here)
//(NOTE: indexes between cmd_names and cmds_ptrs must match)
static uint8_t* cmd_names[] = { "ls",		//CAREFUL WITH MISSING COMMAS	
								"cat",		//gcc will concatenate the string before and after them and compile the project! 
								"date",		//Then it'll jut crash at runtime!...Idk if its legal C, but
								"sysinfo",	//it's a hard-to-find bug.
								SENTINEL_CMD	
								};

uint8_t* day_to_month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
							"Aug", "Sep", "Oct", "Nov", "Dec" };

//The actual commands (Add ptrs to commands here)		
//(NOTE: indexes between cmd_names and cmds_ptrs must match)
static uint32_t* cmd_ptrs[] = { commands_ls,	
								commands_cat,
								commands_date,
								commands_sysinfo,
								};

static uint8_t* newline; 

/*
* Inits the console
*/
void console_init( ){
	//sets newline char to use
	newline = SYS_CONSOLE_NEWLINE;
}


/*
* Prints to the console with a newline at the end
*/
void console_puts( uint8_t* string ){
	print(string);
	print(newline);
}

static void print( uint8_t* string ){
	while( *string )
		hal_io_serial_putc( SerialA, *string++ );
}

/*
*
* Begins the console
*
* Shows the prompt and waits for user input
*
*/
void console_begin( void ){
	
	//wait for any character
	//hal_io_serial_getc(SerialA); //accounts for spurious 0x00 character received
	
	//prints welcome msg
	print_welcome_message();
	
	//creates prompt message
	uint8_t line[MAX_LINE_LEN];
	uint32_t prompt_msg_len =	SYS_USERNAME_MAX_LENGTH	  //user
								+ 1						  //@
								+ strlen(SYS_NAME)		  //sysname
								+ 1						  //:
								+ strlen(DEFAULT_PATH)	  //path
								+ strlen(CONSOLE_SYMBOL)  //symbol
								+ 1						  //' '
								+ 1;					  //null char
	uint8_t prompt_message[ prompt_msg_len ];

	sprintf( prompt_message, "%s@%s:%s%s ", DUMMY_USER, SYS_NAME, DEFAULT_PATH, CONSOLE_SYMBOL );
	
	
	//Begin...
	uint8_t cmd[MAX_COMMAND_LEN + 1];
	uint8_t param[MAX_PARAM_LEN + 1]; //only 1 param is supported for now
	
	while( true ){
		//Shows prompt
		print( prompt_message );

		//reads user input onto buffer
		readln( line );
			
		//extract command and params from input
		extract_command( line, cmd ); 
		uint32_t num_params =  extract_param( line, param );
		
		//try and process as command
		int32_t cmd_index = command_index( cmd );
		
		if( cmd_index != INVALID_CMD ){
			//execute command
			((void(*)(uint8_t*, uint32_t))(cmd_ptrs[cmd_index]))( param, num_params ); 
		}
		else{
			//Not a command. Try and process as an App
			uint32_t sched_rval;
			uint32_t loader_rval;
			
			sched_rval = scheduler_process_create( cmd, cmd, &loader_rval );
			
			if( sched_rval == SCHEDULER_PROCESS_CREATE_SUCCESS ){
				//process creation went fine, halt
				//TO DO: Support for continue executing commands
				while(1);
			}
			else{
				if( loader_rval == LOADER_NO_APP_FOUND) 		console_puts( "Command not recognized" );
				else if( loader_rval == LOADER_APP_TOO_LARGE )	console_puts( "Application is too large" );
				else{
					/* Unknown error*/
				}
			}
		
		}//end if
	}//end while
}

static void print_welcome_message(void){
	//line1, empty lines
	console_puts( "" );
	
	//line2, welcome
	uint8_t welcome_line[ MAX_LINE_LEN ];
	sprintf( welcome_line, "Welcome to %sv%s", SYS_NAME, SYS_VERSION );
	console_puts( welcome_line );
	
	//line3, time
	tTime clock;
	hal_io_clock_read( &clock );
	sprintf( welcome_line, "%s %d %d,  %d hrs %d mins", day_to_month[clock.month-1], clock.day, clock.year, clock.hours, clock.minutes );
	console_puts( welcome_line );
	
	//line4, empty line
	console_puts( "" );
}

static void extract_command( uint8_t* line, uint8_t* cmd ){
	uint8_t c;
	
	//copy everything until a space or null char
	do{
		c = *cmd++ = *line++;
	}while( c != ' ' && c != '\0' );
	
	//turn cmd into a string
	cmd[-1] = '\0';
}

static uint32_t extract_param( uint8_t* line, uint8_t* param ){
	uint8_t c;
	
	//advance line up to end of command
	do{
		c = *line++;
	}while( c != ' ' && c != '\0' );		
	
	if(line[-1] == '\0')
		return 0; //no parameters found
	
	//copy the rest to param (only 1 param is supported for now)
	while( *param++ = *line++ );
	
	//(only 1 param is supported)
	return 1; 
}

static int32_t command_index( uint8_t* cmd ){
	#define EQUAL 0
	uint8_t** all_cmds = cmd_names;
	int32_t index = 0;
	 
	//search for command in the command list
	//and returns its index
	while( strcmp( *all_cmds, SENTINEL_CMD ) != EQUAL ){
		if( strcmp( *all_cmds, cmd ) == EQUAL ){
			//we have a match
			return index; 
		}
		all_cmds++;	
		index++;
	}
	
	return INVALID_CMD; //no match found
}

static void readln( uint8_t* buffer ){

	//read everything until a new line is received
	uint8_t c;
	do{
		c = hal_io_serial_getc(SerialA);
		
		if( c != '\n' && c!= '\r')
			hal_io_serial_putc( SerialA, c );	//transmits most chars for visual feedback
		
		//filters non-alphanumeric(UNION)newline(UNION)' '(UNION)'.' characters
		if( is_alpha(c) || is_number(c) || c == '\n' || c == ' ' || c == '.'  ){
			 *buffer++ = c;
		}
	}while( c != '\n' );

	//turn buffer into a string by replacing the newline character for a null character
	buffer[-1] = '\0';
	
	//go to newline
	console_puts("");
}

static bool is_alpha(uint8_t c){
	return ( c >= 'A' && c<='Z' ) || ( c>= 'a' && c<= 'z' );
}

static bool is_number(uint8_t c){
	return c >= '0' && c<='9';
}


