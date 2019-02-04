/**
 * @file	system.c
 * @author  
 * @version 
 *
 * @brief System module
 *
 * System-related definitions, and definitions that are common 
 * for different layers of the system.
 *
 */

#include "system.h"
#include "HAL/hal.h"
#include "console/console.h"
#include "syscalls.h"
#include "faults.h"
#include "scheduler.h"
#include "loader.h"

#define SCB_CCR_STKALIGN_OFFSET	9	/* Page 217 ATSAM4SD32C Datasheet*/

static void io_init(void);
static void init_sequence(void);
static void println_display( uint8_t* );
static void show_init_info( uint8_t* );

/**
*	System Init
*
*	Initializes IO and CPU. Must be called before any other call
*
*/
void system_init(void){
	
	//Initializes the HAL
	hal_cpu_init();			
	hal_io_init();		
	hal_nvmem_init();
	hal_memreg_init();

	//Inits console
#ifdef SYS_INIT_FROM_CONSOLE
	console_init();
#endif

	//Start SerialA first
	//   NOTE:  Printing to console and System panics allowed past this point.
	//          (Can't do it before both console and SerialB are initialized)
	hal_io_serial_start_poll( SerialA, SYS_CONF_SERIAL_A_BAUDRATE );
	
	//Initializes IO devices
	show_init_info( "" );							io_init();
		
	//Inits remaining modules
	show_init_info( "Initializing Faults" );		faults_init();
	show_init_info( "Initializing Syscalls" );		syscalls_init();
	show_init_info( "Initializing Loader" );		loader_init();
	show_init_info( "Initializing Scheduler" );		scheduler_init();

	//Sets the stack as 8-byte aligned on exception entry (see Documentation pg. 217)
	uint32_t ccr = hal_cpu_get_scb_ccr() | (1<< SCB_CCR_STKALIGN_OFFSET);
	hal_cpu_set_scb_ccr( ccr );
	
}

/**
*	Kernel Panic Function
*
*	Prints a Kernel panic message and puts the CPU to sleep. Template
*	from https://en.wikipedia.org/wiki/Kernel_panic
*
*	@param manic_msg the panic message
*
*/
void system_panic( const uint8_t* panic_msg ){
	
	//report msg on Display
#ifdef SYS_IO_DISPLAY_PRESENT
	hal_io_display_cls();
	println_display( ":( Something went wrong..." );
	println_display( "" );
	println_display( panic_msg );
#endif

	//report msg on Serial Console
#ifdef SYS_INIT_FROM_CONSOLE
	console_puts( "" );
	console_puts( ":( Something went wrong..." );
	console_puts( panic_msg );
#endif

	//Show panic LED sequence.
	bool led_state = false;
	while( true ){
		//toggle all board LEDs
		// FIX THIS WHEN HAL IS FINISHED
		//for( uint32_t i=0; i<5; i++ )
		//hal_io_led_write( i, led_state = !led_state );
		
		//pause
		//hal_cpu_delay(150);
	}
}

static void io_init(void){
	
	//Starts All IO devices
	show_init_info( "Starting PWM" );			hal_io_pwm_start();
	show_init_info( "Starting Parallel IO" );	hal_io_pio_start();
#ifdef SYS_IO_DISPLAY_PRESENT
	show_init_info( "Starting Display" );		hal_io_display_start();
#endif
	show_init_info( "Starting SerialB" );		hal_io_serial_start_poll( SerialB, SYS_CONF_SERIAL_A_BAUDRATE );
	show_init_info( "Starting Light Sensor" );	hal_io_sensor_start_poll( SensorLight );
	show_init_info( "Starting Temp Sensor" );	hal_io_sensor_start_poll( SensorTemp );
	show_init_info( "Starting Clock" );			hal_io_clock_start();
	
	//Clock starts at 00:13:00 July 14, 1991
	tTime time = {
		0, 13, 0, 14, 7, 1991 
	};
	
	hal_io_clock_write(&time);
	
	//Starts Non-volatile memory
	show_init_info( "Starting Non-volatile Memory" );
	if( !hal_nvmem_start( NVMemSDCardFAT ) )
		system_panic( "FS failed to mount" );
	
	//Visual cue 
	init_sequence();

}

static void init_sequence(void){
		
	//All LEDs On
	// FIX THIS WHEN HAL FINISHED
	//hal_io_led_write(Led0, LedOn);
	//hal_io_led_write(Led1, LedOn);
	//hal_io_led_write(Led2, LedOn);
	//hal_io_led_write(Led3, LedOn);
	//hal_io_led_write(Led4, LedOn);
		
	//Screen "on"
#ifdef SYS_IO_DISPLAY_PRESENT
	for(uint32_t i=0; i<4; i++){	 //rows
		hal_io_display_gotoxy(i, 0);
		for(uint32_t j=0; j<20; j++) //columns
			hal_io_display_putc('#');
	}
#endif

	//Wait...
	hal_cpu_delay(50);
	
	//All LEDs Off
	// FIX THIS
	//hal_io_led_write(Led0, LedOff);
	//hal_io_led_write(Led1, LedOff);
	//hal_io_led_write(Led2, LedOff);
	//hal_io_led_write(Led3, LedOff);
	//hal_io_led_write(Led4, LedOff);
		
	//clear screen
#ifdef SYS_IO_DISPLAY_PRESENT
	hal_io_display_cls();
#endif
}

static void println_display( uint8_t* string ){
	while( *string )
		hal_io_display_putc( *string++ );
	//go to newline
	hal_io_display_gotoxy( hal_io_display_currline()+1, 0 );
}

static void show_init_info( uint8_t* string ){
	
#ifdef SYS_INIT_FROM_CONSOLE
	console_puts( string );
#endif

}