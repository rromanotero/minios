#include <stdint.h>
#include "minilib/display.h"
#include "minilib/serial.h"
#include "stdio/stdio.h"
//#include "minilib/dummy.h"

//#include <string.h>

uint32_t dummy2(){

  uint32_t some_val;
  uint32_t* p_some_val = &some_val;

  asm volatile(
    "mov  r0, %[result] \n"
    "svc #1" //Dummy Syscall (just fo testing)
    :[result] "=r" (p_some_val)
    :
    : "lr" // <<<--- dont'r remove this!!!!!
  );

  return some_val;
}

int main( void ){


  //for(volatile long i=0; i<1000000000;i++);

  printf( "Hello there. This is a process!\n\r" );

  asm volatile(
    "mov  r0, #7 \n"
    "mov  r1, #7 \n "
    "mov  r2, #7 \n"
    "svc #0" //Dummy Syscall (just fo testing)
  );

  uint32_t some_val = dummy2();

  printf( "I received back from the syscall: %d", some_val );

  /*uint8_t dst[10];
  uint8_t src[10];

  memcpy( dst, src, 10 );*/

  //for(volatile long i=0; i<1000000000;i++);

  //NOte:
  //  I haven't tested getc() but it shoudln't be used
  //  as it'll make busy-wait for a character input on
  //  the SVC handler

  return 0;
}
