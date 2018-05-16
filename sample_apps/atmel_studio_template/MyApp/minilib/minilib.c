/*
 * minilib.c
 *
 * C part of Minilib.
 *
 */ 

#include "display.h"
#include "serial.h"
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>

#define DISPLAY_MAX_PRINT_LEN	120		//characters that fit in the screen
#define SERIAL_MAX_PRINT_LEN	255		//arbitrarily set to 255
#define MAX_INT_LEN				10		//max characters an int can have

static bool print_aux_display(const uint8_t*, size_t, size_t );
static bool print_aux_serial( tSerialId, const uint8_t*, size_t, size_t );
static void newline_display(void);


/*
 * Display Puts
 *
 * Prints a string (+ a newline) on the display
*/
void display_puts( const uint8_t* string ){	
	//print strings
	uint8_t* pStr = string;
	while( *pStr )
		display_putc( *pStr++ );
	
	//newline
	newline_display();
}

/*
 * Display Printf
 *
 * Formatted printing on the Display. At the moments supports
 * 'd', 'c', and 's'.
 *
 * Won't print beyond DISPLAY_MAX_PRINT_LEN characters
 * and won't print integers past DISPLAY_MAX_INT_LEN characters
 *
 * Adapted from skeleton in:
 *    http://wiki.osdev.org/Meaty_Skeleton#libc.2Fstdio.2Fprintf.c
*/
void display_printf( const uint8_t* format, ... ) {
	va_list args;
	va_start( args, format );
	
	size_t used = 0;
	size_t remaining = 0;
	
	while( *format ){
		remaining = DISPLAY_MAX_PRINT_LEN - used;
		
		if( format[0] != '%' || format[1] == '%' ){	
			
			if (format[0] == '%')
				format++;
					
			//fetch (from this point up to the next %)
			size_t len = 1;
			while ( format[len] && format[len] != '%'  )
				len++;
			
			//print
			if( !print_aux_display(format, len, remaining) )
				return;
				
			//next
			format += len;
			used += len;
			continue;
		}

		//A placeholder
		const uint8_t* marker = format++;
		
		if( *format == 'c' ) {	
			// -- Character --
			
			//fetch
			uint8_t c = (uint8_t) va_arg( args, uint32_t );
			
			//print
			if( !print_aux_display( &c, 1, remaining ) )
				return;
			
			//next
			used++;
			format++;
		} else if( *format == 'd' ) {		
			// -- Integer-- 
			
			//fetch
			uint32_t v = (uint32_t) va_arg( args, uint32_t );
			uint8_t buff[MAX_INT_LEN+1];
			itoa( v, buff, 10 );
			
			//print
			size_t len = strlen(buff);
			if( !print_aux_display(buff, len, remaining) )
				return;
				
			//next
			used += len;
			format++;
		} else if ( *format == 's' ) {
			// -- String --
			
			//fetch
			const uint8_t* str = va_arg(args, const uint8_t*);
			
			//print
			size_t len = strlen(str);
			if( !print_aux_display(str, len, remaining) )
				return;
			
			//next
			used += len;
			format++;
			
		} else {
			//Not too sure what this prints...
			// % symbols? unsupported placeholders?
			// but then why would it print the rest of the format???
			
			//fetch
			format = marker;
			size_t len = strlen(format);
			
			//print
			if( !print_aux_display(format, len, remaining) )
				return;
				
			//next
			used += len;
			format += len;
		}//end if-else
	}//end while
	
	//clean
	va_end(args);
}

//
// Returns false if no space left to print, true otherwise
//
static bool print_aux_display(const uint8_t* data, size_t length, size_t remaining) {
	if( remaining < length ){
		//not enough space to print
		return false; 
	}
		
	//print string
	for (size_t i = 0; i < length; i++){
		if( data[i] == '\n' ) //TO DO: change this later, don't want to check 
			newline_display();		  //        for newlines on every character printed
		else
			display_putc( data[i] );
	}
	
	return true;
}

static void newline_display(void){
	//feed + carriage return
	display_gotoxy( (display_currline() + 1) % display_numlines(), 0 );
}

/*
 * Serial Puts
 *
 * Prints a string (+ a newline) on the serial device with id serial_id
*/
void serial_puts( tSerialId serial_id, const uint8_t* string ){	
	//print string
	uint8_t* pStr = string;
	while( *pStr )
		serial_putc( serial_id, *pStr++ );
	
	//newline
	serial_putc( serial_id, '\n' );
}

/*
 * Serial Printf
 *
 * Formatted printing on the Display. At the moments supports
 * 'd', 'c', and 's'.
 *
 * Won't print beyond DISPLAY_MAX_PRINT_LEN characters
 * and won't print integers past DISPLAY_MAX_INT_LEN characters
 *
 * Adapted from skeleton in:
 *    http://wiki.osdev.org/Meaty_Skeleton#libc.2Fstdio.2Fprintf.c
*/
void serial_printf( tSerialId serial_id, const uint8_t* format, ... ) {
	va_list args;
	va_start( args, format );
	
	size_t used = 0;
	size_t remaining = 0;
	
	while( *format ){
		remaining = SERIAL_MAX_PRINT_LEN - used;
		
		if( format[0] != '%' || format[1] == '%' ){	
			
			if (format[0] == '%')
				format++;
					
			//fetch (from this point up to the next %)
			size_t len = 1;
			while ( format[len] && format[len] != '%'  )
				len++;
			
			//print
			if( !print_aux_serial( serial_id, format, len, remaining ) )
				return;
				
			//next
			format += len;
			used += len;
			continue;
		}

		//A placeholder
		const uint8_t* marker = format++;
		
		if( *format == 'c' ) {	
			// -- Character --
			
			//fetch
			uint8_t c = (uint8_t) va_arg( args, uint32_t );
			
			//print
			if( !print_aux_serial( serial_id, &c, 1, remaining ) )
				return;
			
			//next
			used++;
			format++;
		} else if( *format == 'd' ) {		
			// -- Integer-- 
			
			//fetch
			uint32_t v = (uint32_t) va_arg( args, uint32_t );
			uint8_t buff[MAX_INT_LEN+1];
			itoa( v, buff, 10 );
			
			//print
			size_t len = strlen(buff);
			if( !print_aux_serial( serial_id, buff, len, remaining ) )
				return;
				
			//next
			used += len;
			format++;
		} else if ( *format == 's' ) {
			// -- String --
			
			//fetch
			const uint8_t* str = va_arg(args, const uint8_t*);
			
			//print
			size_t len = strlen(str);
			if( !print_aux_serial( serial_id, str, len, remaining ) )
				return;
			
			//next
			used += len;
			format++;
			
		} else {
			//Not too sure what this prints...
			// % symbols? unsupported placeholders?
			// but then why would it print the rest of the format???
			
			//fetch
			format = marker;
			size_t len = strlen(format);
			
			//print
			if( !print_aux_serial( serial_id, format, len, remaining) )
				return;
				
			//next
			used += len;
			format += len;
		}//end if-else
	}//end while
	
	//clean
	va_end(args);
}

//
// Returns false if no space left to print, true otherwise
//
static bool print_aux_serial( tSerialId serial_id,  const uint8_t* data, size_t length, size_t remaining) {
	if( remaining < length ){
		//not enough space to print
		return false; 
	}
		
	//print string
	for (size_t i = 0; i < length; i++)
			serial_putc( serial_id, data[i] );
	
	return true;
}

