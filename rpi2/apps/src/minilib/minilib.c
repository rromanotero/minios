/*
 * minilib.c
 *
 * C part of Minilib.
 *
 */

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>

#include "display.h"
#include "serial.h"
#include "dummy.h"


uint32_t dummy( uint32_t a, uint32_t b, uint32_t c ){
    uint32_t ret_val;

    asm volatile(
      "mov r3, %[input] \n"
      "svc #0"
      : //output
      : [input] "r" (&ret_val) //input
      : "lr"  //Clobber registers
    );

    return ret_val;
}
