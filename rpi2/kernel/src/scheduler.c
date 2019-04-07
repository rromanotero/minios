#include "hal/hal.h"
#include "scheduler.h"


#define TICK_FREQ		1000

__attribute__((naked)) static void tick_callback(void);


/*
*	Scheduler Init
*
*   Initializes the scheduler. The system timer is not started here.
*/
void scheduler_init(void){

	hal_cpu_systimer_start( TICK_FREQ, tick_callback );

}

/*
*	The infamous tick callback
*
*	Context switch takes place here.
*/
__attribute__((naked)) static void tick_callback(void){
	/*
			In the PI implementation it doesn't need to be NAKED
			We've alreadyoushed everything onto the stack
	*/
	hal_cpu_systimer_reestart( TICK_FREQ ); //Do it at the beginning
																		 			//to reduce driftoff

  hal_io_video_puts( "tick ", 1, VIDEO_COLOR_RED );
	//hal_io_serial_puts( SerialA, "tick" );

}
