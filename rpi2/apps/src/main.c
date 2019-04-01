#include <stdint.h>
#include "minilib/display.h"
#include "minilib/serial.h"
#include "stdio/stdio.h"
#include "minilib/dummy.h"

int main( void ){


  for(volatile long i=0; i<1000000000;i++);

  printf( "Hello there. This is a process!\n\r" );

  asm volatile(
    "mov  r0, #7 \n"
    "mov  r1, #7 \n "
    "mov  r2, #7 \n"
    "svc #0" //Dummy Syscall (just fo testing)
  );

  uint32_t some_val = dummy2();

  printf( "I received back from the syscall: %d", some_val );

  for(volatile long i=0; i<1000000000;i++);

  //NOte:
  //  I haven't tested getc() but it shoudln't be used
  //  as it'll make busy-wait for a character input on
  //  the SVC handler

  return 0;
}
