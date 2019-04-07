
/*
*
*	The Kernel
*
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "irq.h"

#include "kernel.h"
#include "hal/hal.h"
#include "console/console.h"
#include "loader.h"
#include "syscalls.h"
#include "scheduler.h"

#include "drivers/stdio/emb-stdio.h"			// Needed for printf
#include "drivers/sdcard/SDCard.h"

void kernel_init(void);
void input_output_init(void);
void sys_info( uint8_t* );


/*
 *		Kernel's entry point
**/
void main(uint32_t r0, uint32_t r1, uint32_t atags){

  //Init
  kernel_init();
  input_output_init();

  console_begin();

	while (1);
}

/*
* Initializes the kernel
*/
void kernel_init(void){

  hal_io_init();
  hal_nvmem_init();
  hal_memreg_init();
  console_init();
  loader_init();
  syscalls_init();
  irq_init();
  scheduler_init();
  //faults_init();


  hal_nvmem_start( NVMemSDCardFAT );

}

/*
* Initializes All IO
*/
void input_output_init(void){
  uint32_t video_init_res = HAL_FAILED;

#ifdef VIDEO_PRESENT
  video_init_res = hal_io_video_init();

  if ( video_init_res == HAL_SUCCESS )
    sys_info( "Video Initialized\n\r" );
  else
    sys_info( "Video Init Failed [x]\n\r" );
#endif

#ifdef SERIAL_PRESENT
  hal_io_serial_init();
  sys_info( "Serial Initialized\n\r" );
#endif
//NOTE: PAST THIS POINT WE CAN USE printf
//      (printf needs both serial and video inits to be called first)

}

void sys_info( uint8_t* msg ){
#ifdef VIDEO_PRESENT
  printf_video( msg );
#endif
#ifdef SERIAL_PRESENT
  printf_serial( msg );
#endif
}
