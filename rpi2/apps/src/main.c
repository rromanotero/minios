#include <stdint.h>
#include "minilib/display.h"
#include "minilib/serial.h"
#include "stdio/stdio.h"

int main( void ){

  printf( "Hello there. This is a process!" );

  //NOte:
  //  I haven't tested getc() but it shoudln't be used
  //  as it'll make busy-wait for a character input on
  //  the SVC handler

  return 0;
}
