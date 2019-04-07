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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../hal/hal.h"
#include "../drivers/stdio/emb-stdio.h"
#include "console.h"
#include "commands.h"
#include "../kernel.h"
#include "../loader.h"
#include "../drivers/per_core_timer/per_core_timer.h"

static void print( uint8_t* );
static void readln( uint8_t* );
static void extract_command( uint8_t*, uint8_t* );
static uint32_t extract_param( uint8_t*, uint8_t* );
static int32_t command_index( uint8_t* );
static bool is_alpha( uint8_t );
static bool is_number( uint8_t );
static void print_welcome_message(void);
static void show_logo(void);

#define MAX_COMMAND_LEN			20			//command length (arbitrary)
#define MAX_PARAM_LEN				20			//param length (arbitrary)
#define MAX_LINE_LEN				MAX_COMMAND_LEN + MAX_PARAM_LEN*2 + 1	//one command + 2 params + null char
#define MAX_NUM_OF_PARAMS		1				//Only 1 param is supported, for now.

#define DUMMY_USER					"user"	//No support for users yet
#define DEFAULT_PATH				"~"			//Initially the system is in the root directory. No support for dirs yet.
#define CONSOLE_SYMBOL			"$"			//The symbol that shows before the prompt
#define INVALID_CMD					-1
#define SENTINEL_CMD				"[[]]"		//some string that won't match any command
																			//Don't know what a sentinel is? See https://en.wikipedia.org/wiki/Sentinel_value

//List of command names (Add new commands here)
//(NOTE: indexes between cmd_names and cmds_ptrs must match)
static uint8_t* cmd_names[] = { "ls",				//CAREFUL WITH MISSING COMMAS
																"cat",			//gcc will concatenate the string before and after them and compile the project!
																"date",			//Then it'll jut crash at runtime!...Idk if its legal C, but
																"sysinfo",	//it's a hard-to-find bug.
																"dump",
																"cd",
																SENTINEL_CMD
																};

uint8_t* day_to_month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
														"Aug", "Sep", "Oct", "Nov", "Dec" };

//The actual commands (Add ptrs to commands here)
//(NOTE: indexes between cmd_names and cmds_ptrs must match)
static void (*cmd_ptrs[])(uint8_t *, uint32_t) = { commands_ls,
																									commands_cat,
																									commands_date,
																									commands_sysinfo,
																									commands_dump,
																									commands_cd
																									};
static uint8_t newline;

/*
* Inits the console
*/
void console_init( ){
	commands_init();

	//sets newline char to use
	newline = SYS_CONSOLE_NEWLINE;
}


/*
* Prints to the console with a newline at the end
*/
void console_puts( uint8_t* string ){
	hal_io_video_puts( string, 2, VIDEO_COLOR_WHITE );
	//hal_io_video_putc( newline, 2, VIDEO_COLOR_WHITE );

	hal_io_serial_puts( SerialA, string );
	//hal_io_serial_putc( SerialA, newline );
}

int console_printf (uint8_t* fmt, ...){
	return printf_video(fmt);
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

	//Show MiniOS Logo
	show_logo();

	//prints welcome msg
	print_welcome_message();

	//creates prompt message
	uint8_t line[MAX_LINE_LEN];
	uint32_t prompt_msg_len =	SYS_USERNAME_MAX_LENGTH	  	//user
															+ strlen(CONSOLE_SYMBOL)  //symbol
															+ 1						  					//' '
															+ 1;					  					//null char
	uint8_t prompt_message[ prompt_msg_len ];

	sprintf( prompt_message, "%s%s ", DUMMY_USER, CONSOLE_SYMBOL );


	//Begin...
	uint8_t cmd[MAX_COMMAND_LEN + 1];
	uint8_t param[MAX_PARAM_LEN + 1]; //only 1 param is supported for now

	while( true ){
		//Shows prompt
		console_puts( "\n\r" );
		hal_io_video_puts( prompt_message, 2, VIDEO_COLOR_GREEN );
	  hal_io_serial_puts( SerialA, prompt_message );

		//reads user input onto buffer
		readln( line );
		console_puts( "\n\r" );

		//extract command and params from input
		extract_command( line, cmd );
		uint32_t num_params =  extract_param( line, param );

		//try and process as command
		int32_t cmd_index = command_index( cmd );

		if( cmd_index != INVALID_CMD ){
			//execute command
			//console_puts( "No commands supported yet\n" );
			((void(*)(uint8_t*, uint32_t))(cmd_ptrs[cmd_index]))( param, num_params );
		}
		else{
			//Not a command. Try and process as an App

			tMemRegion proc_memregion;
			uint32_t stack_sz;

			//Load app binary
			uint32_t rval = loader_load_app( cmd, &proc_memregion, &stack_sz );
			if(  rval == LOADER_NO_APP_FOUND	 ){
				console_puts( "App not found" );
				continue;
			}
			else if(  rval == LOADER_APP_TOO_LARGE	 ){
				console_puts( "App too big" );
				continue;
			}

			//M I S S I N G:
			//  Set up User stack
			//  Switch to user mode

			//Run App
			uint32_t ret_val = ((int(*)(void))(proc_memregion.base))();

			if( ret_val != 0){
				printf_serial( "\n\rProcess exited with value %d", ret_val );
				printf_video( "\n\rProcess exited with value %d", ret_val );
			}

			continue;

		}//end if
	}//end while
}

#define MAX_FILE_SIZE	700000
uint8_t buffer_logo[MAX_FILE_SIZE];

static void show_logo(void){

	//checks file
	uint32_t size = hal_nvmem_fat_file_size( SYS_LOGO_FILENAME );
	if( size == 0 ){
		uint8_t msg[30];
		sprintf( msg, "\n\rFailed to display logo \n\r%s not found\n\r", SYS_LOGO_FILENAME );
		hal_io_video_puts( msg, 2, VIDEO_COLOR_RED );
		return;
	}

	//reads
	uint32_t bytes_read =  hal_nvmem_fat_file_read( SYS_LOGO_FILENAME, buffer_logo, MAX_FILE_SIZE );

	//display
	hal_io_video_draw_image( (uint8_t*)buffer_logo, bytes_read, SYS_LOGO_WIDTH, SYS_LOGO_HEIGHT );

}

static void print_welcome_message(void){

	//line1, empty lines
	console_puts( "" );

	//line2, welcome
	uint8_t welcome_line[ MAX_LINE_LEN ];
	sprintf( welcome_line, "\n\r\n\rWelcome to %s %s\n\r", SYS_NAME, SYS_VERSION );

	hal_io_video_puts( welcome_line, 3, VIDEO_COLOR_GREEN );
  hal_io_serial_puts( SerialA, welcome_line );

	//line3, time
	/*tTime clock;
	hal_io_clock_read( &clock );
	sprintf( welcome_line, "%s %d %d,  %d hrs %d mins", day_to_month[clock.month-1], clock.day, clock.year, clock.hours, clock.minutes );
	console_puts( welcome_line );
	*/

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

		if( c != newline ){
			//transmits most chars for visual feedback
			hal_io_video_putc( c, 2, VIDEO_COLOR_WHITE );
			hal_io_serial_putc( SerialA, c );
		}

		//filters non-alphanumeric(UNION)newline(UNION)' '(UNION)'.' characters
		if( is_alpha(c) || is_number(c) || c == newline || c == ' ' || c == '.'  ){
			 *buffer++ = c;
		}
	}while( c != newline );

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
