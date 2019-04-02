#include <stdint.h>
#include "minilib/display.h"
#include "minilib/serial.h"
#include "stdio/stdio.h"
#include "minilib/dummy.h"

int main( void ){

  //This is a dummy syscall...
  //Just foe reference (see slides for week 13 part two)
  uint32_t received =  dummy( 7, 7, 7 );

  printf( "I received: %d", received );

  return 0;
}
