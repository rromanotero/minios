

#include "hal/hal.h"
#include "irqs.h"
#include "hal/hal.h"

void irqs_entry_point(void);

/**
*	Interrupts Init
*
*	Initializes the interrupts interface
*
*/
void irqs_init(void){
	//Starts Interrupts
	hal_cpu_irqs_start( irqs_entry_point );
}

void irqs_entry_point(void){


  hal_io_video_puts( "TICK", 4, VIDEO_COLOR_RED );

}
