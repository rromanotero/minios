/*
 * minilib.c
 *
 * Assembly part of Minilib
 */

.section .text
.align 4

//Serial
.equ	SVCSerialPutc,	  	 9
.equ	SVCSerialGetc,		   10

//Display
.equ	SVCDisplayPutc,		   15


/*
	For details on how to receive data
	from the OS see: https://embedntks.com/datastructs/resources/syscall-return-hint.png
*/


/*
	Serial
*/
.globl serial_putc
serial_putc:
  push {lr}   /* We need prologue and epilogue when Apps run in kernel mode (since kernel and app share the STack!)*/
	svc SVCSerialPutc
  pop {pc}

.globl serial_getc
serial_getc:
	/* prologue */
	push {lr}
	sub sp, #4				     /* uint8_t c; */

	mov r1, sp
	bl serial_getc_aux		/* serial_read( serial_id, &c ); */

	ldrb r0, [sp]			     /* return (uint8_t)c;  */

	/* epilogue */
	add sp, #4
	pop {pc}

serial_getc_aux:
	svc SVCSerialGetc
	bx lr


/*
	Display
*/
.globl display_putc
display_putc:
  push {lr}   /* We need prologue and epilogue when Apps run in kernel mode (since kernel and app share the STack!)*/
	svc SVCDisplayPutc
  pop {pc}
