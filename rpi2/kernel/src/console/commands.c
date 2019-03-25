/**
 * @file	commands.c
 * @author
 * @version
 *
 * @brief The commands module part of the console. Contains definitions for commands available in the
 *		  console.
 *		  Note: only one param (command-line argument) is supported.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "string.h"

#include "../hal/hal.h"
#include "console.h"
#include "commands.h"
#include "../kernel.h"
#include "../drivers/stdio/emb-stdio.h"


/*
*
*/
#define MAX_PATH_LENGTH	100
uint8_t curr_path[MAX_PATH_LENGTH];

void commands_init(void){
	//Init the current path
	for(uint32_t i=0; i<MAX_PATH_LENGTH; i++)
		curr_path[i] = '\0';
	sprintf( curr_path, "%s", "/" );

}

void commands_cd( uint8_t* param, uint32_t num_params ){

	//checks param
	if( num_params == 0){
		console_puts( "cd: path missing" );
		return;
	}

	//check if dir exists
  //missing...

	if( strcmp( param, ".." ) == 0 ){
			//process .. (up one level)
			uint8_t* p_last = curr_path + strlen(curr_path);	//start off at the end of the path
			p_last--;																					//skip the last /
			while( *p_last-- != '/' ); 												//find the end of one level up
			*(p_last+1) = '\0';																//remove everything after (so cd ..)
	}
	else{
			//update curr_path
			sprintf( curr_path, "%s%s/", curr_path, param );
	}

	console_puts( "current dir: " );
	console_puts( curr_path );
	console_puts( " (this is in development still!!)\n" );
}

void commands_dump( uint8_t* param, uint32_t num_params ){

	#define MAX_CHARS_IN_FILE_DUMP	2048*2 //4Kb for now

	//checks param
	if( num_params == 0){
		console_puts( "dump: filename missing" );
		return;
	}

	uint32_t size = hal_nvmem_fat_file_size(param);
	//checks file
	if( size == 0 ){
		console_puts( "dump: file does not exist" );
		return;
	}
	/*else if( size > MAX_CHARS_IN_FILE_DUMP ){
		 console_puts( "dump: only files of size 4Kb or less are supported" );
		 return;
	*/

	//  I M P O R T A N T
	//(CHECKING FILE  SIZE IS NOT CURRENTLY WORKING)
	// so I'm just setting it to 256 for now
	size = 256;


	// -- read binary --
	//reads (bytes_read not working)
	uint8_t buffer[MAX_CHARS_IN_FILE_DUMP+1];
	uint32_t bytes_read =  hal_nvmem_fat_file_read( param, buffer, MAX_CHARS_IN_FILE_DUMP );

	//print
	#define LINE_LENGTH		10
	int i=0;
	for( i=0; i<(size-LINE_LENGTH); i+=LINE_LENGTH ) {
		//for every line

		//dump as hex
		for(	int j=0; j<LINE_LENGTH; j++ ){
			printf_serial( "%02x ", buffer[ i+j ] );
			printf_video( "%02x ", buffer[ i+j ] );
		}

		//dump as ascii
		for(	int j=0; j<LINE_LENGTH; j++ ) {
			uint8_t byte = buffer[ i+j ];

			if( byte == '\n' || byte == '\r' )
				continue;

			if( (byte >= 'a' && byte <= 'z') ||
			 		(byte >= 'A' && byte <= 'Z') ||
					(byte >= '0' && byte <= '9')
			  ){
				printf_serial( "%c", byte );
				printf_video( "%c", byte );
			}
			else{
				printf_serial( " " );
				printf_video( " " );
			}

		}

		//next line
		printf_serial( "\n\r" );
		printf_video( "\n\r" );
	}

	//print last line
	//dump as hex
	for(	int j=i; j<size; j++ ){
		printf_serial( "%02x ", buffer[ i+j ] );
		printf_video( "%02x ", buffer[ i+j ] );
	}

	//dump as ascii
	for(	int j=i; j<size; j++ ) {
		uint8_t byte = buffer[ i+j ];

		if( byte == '\n' || byte == '\r' )
			continue;

		if( (byte >= 'a' && byte <= 'z') ||
				(byte >= 'A' && byte <= 'Z') ||
				(byte >= '0' && byte <= '9')
			){
			printf_serial( "%c", byte );
			printf_video( "%c", byte );
		}
		else{
			printf_serial( " " );
			printf_video( " " );
		}

	}

	printf_serial( "\n\r" );
	printf_video( "\n\r" );

}

/*
*
*/
void commands_ls( uint8_t* param, uint32_t num_params ){
	#define NUM_OF_FILES_TO_READ	20	//arbitrary

	uint8_t file_list[NUM_OF_FILES_TO_READ][ MEM_FAT_MAX_FNAME_LENGTH + 1 ];
	uint8_t line_to_print[ MEM_FAT_MAX_FNAME_LENGTH + strlen("       ") + 9 + strlen( " Bytes" )  + 1 ];  //assuming a file cant have more than 9 digits of size in KB
	uint32_t num_files;
	double size_of_file;

	//print file list
	if( ( num_files = hal_nvmem_fat_file_list( file_list, NUM_OF_FILES_TO_READ ) ) > 0 ){

		for( uint32_t i=0; i<num_files; i++ ){
			//finds file size
			//size_of_file = hal_nvmem_fat_file_size( file_list[i] );
			//size_of_file = size_of_file/1024.0;

			//prepares line to be printed
			//sprintf( line_to_print, "%s       %d KB", file_list[i], size_of_file );
			sprintf( line_to_print, "%s", file_list[i] );

			//print
			console_puts( line_to_print );
			console_puts( "\n\r" );
		}
	}
	else{
		console_puts( "No files found\n\r" );
	}
}

/*
*
*/
void commands_cat( uint8_t* param, uint32_t num_params ){

	#define MAX_CHARS_IN_FILE	2048 //2Kb for now
	uint8_t buffer[MAX_CHARS_IN_FILE];

	//checks param
	if( num_params == 0){
		console_puts( "cat: filename missing" );
		return;
	}

	//checks file
	uint32_t size = hal_nvmem_fat_file_size(param);
	if( size == 0 ){
		console_puts( "cat: file does not exist" );
		return;
	}


	//reads
	uint32_t bytes_read =  hal_nvmem_fat_file_read( param, buffer, MAX_CHARS_IN_FILE );

	//turns buffer into a string
	buffer[bytes_read-1] = '\0';

	//print line by line
	console_puts( buffer );
	console_puts( "\n\r" );
}

/*
*
*/
void commands_date( uint8_t* param, uint32_t num_params  ){

	#define MAX_DATE_LENGTH	26
	//tTime t;
	uint8_t* months[] = {	"January", "February", "March", "April", "May",
												"June", "July", "August", "September", "October",
												"November", "December"  };

	//We're going to fake this, because I don't know if the PI has a hardware clock
	//hal_io_clock_read(&t);


	uint8_t line[MAX_DATE_LENGTH + 1];
	sprintf( line, "%s %d, %d %02d:%02d:%02d", months[7-1], 14, 1991, 0, 13, 0 );

	console_puts( line );
	console_puts( "\n\r" );
}

/*
*
*/
void commands_sysinfo( uint8_t* param, uint32_t num_params  ){
	#define MAX_INFO_LENGTH 15
	uint8_t line[MAX_INFO_LENGTH + 1];
	sprintf( line, "%s %s", SYS_NAME, SYS_VERSION );

	console_puts( line );
	console_puts( "\n\r" );
}
