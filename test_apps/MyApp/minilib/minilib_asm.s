/*
 * minilib.c
 *
 * Assembly part of Minilib
 */ 

//LED
.equ	SVCLedWrite,		0
.equ	SVCLedRead,			1
//Button
.equ	SVCButtonStartallEv,	2
.equ	SVCButtonStartallPoll,	3
.equ	SVCButtonRead,		4
//Clock
.equ	SVCClockRead,		5
.equ	SVCClockWrite,		6
//Serial
.equ	SVCSerialStartEv,	7
.equ	SVCSerialStartPoll, 8
.equ	SVCSerialPutc,		9
.equ	SVCSerialGetc,		10
//Sensors
.equ	SVCSensorStartEv,	11
.equ	SVCSensorStartPoll, 12
.equ	SVCSensorRead,		13
//Display
.equ	SVCDisplayCls,		14
.equ	SVCDisplayPutc,		15
.equ	SVCDisplayGotoxy,	16
.equ	SVCDisplayNumLines, 17
.equ	SVCDisplayCurrLine, 18
//Millisecond timer
.equ	SVCMtimerStartEv,	19
.equ	SVCMtimerStartPoll, 20
.equ	SVCMtimerStop,		21
.equ	SVCMtimerRead,		22
//System
.equ	SVCSysinfo,			23

/* 
	For details on how to receive data 
	from the OS see: https://embedntks.com/datastructs/resources/syscall-return-hint.png
*/


/* 
	Serial
*/
.thumb_func 
.global serial_start_poll
serial_start_poll:
	svc SVCSerialStartPoll
	bx lr

.thumb_func 
.global serial_putc
serial_putc:
	svc SVCSerialPutc
	bx lr

.thumb_func 
.global serial_getc
serial_getc:
	/* prologue */
	push {lr}
	sub sp, #4				/* uint8_t c; */

	mov r1, sp
	bl serial_getc_aux		/* serial_read( serial_id, &c ); */

	ldrb r0, [sp]			/* return (uint8_t)c;  */

	/* epilogue */
	add sp, #4
	pop {pc}

serial_getc_aux:
	svc SVCSerialGetc
	bx lr

/*
	Sensors
*/
.thumb_func 
.global sensor_start_poll
sensor_start_poll:
	svc SVCSensorStartPoll
	bx lr

.thumb_func 
.global sensor_read
sensor_read:
	/* prologue */
	push {lr}
	sub sp, #4				/* uint32_t sensor_val; */

	mov r1, sp
	bl sensor_read_aux		/* sensor_read( sensor_num, &sensor_val ); */

	ldr r0, [sp]			/* return (uint32_t)sensor_val;  */

	/* epilogue */
	add sp, #4
	pop {pc}

sensor_read_aux:
	svc SVCSensorRead
	bx lr



/*
	Buttons
*/
.thumb_func 
.global button_startall_poll
button_startall_poll:
	svc SVCButtonStartallPoll
	bx lr

.thumb_func 
.global button_read
button_read:
	/* prologue */
	push {lr}
	sub sp, #4				 /* tButtonState state */

	mov r1, sp
	bl button_read_aux		/* button_read( button_num, &state ); */

	ldrb r0, [sp]			 /* return (tButtonState)state;  */

	/* epilogue */
	add sp, #4
	pop {pc}

button_read_aux:
	svc SVCButtonRead
	bx lr


/*
	Timer
*/
.thumb_func 
.global mtimer_start_poll
mtimer_start_poll:
	svc SVCMtimerStartPoll
	bx lr

.thumb_func 
.global mtimer_stop
mtimer_stop:
	svc SVCMtimerStop
	bx lr

.thumb_func 
.global mtimer_read
mtimer_read:
	/* prologue */
	push {lr}
	sub sp, #4				 /* uint32_t timer_val */

	mov r0, sp
	bl mtimer_read_aux		/* mtimer_read_aux( &timer_val ); */

	ldr r0, [sp]			 /* return (uint32_t)timer_val;  */

	/* epilogue */
	add sp, #4
	pop {pc}

mtimer_read_aux:
	svc SVCMtimerRead
	bx lr

/*
	Clock
*/
.thumb_func 
.global clock_write
clock_write:
	svc SVCClockWrite
	bx lr

.thumb_func 
.global clock_read
clock_read:
	svc SVCClockRead
	bx lr


/* 
	Display
*/
.thumb_func 
.global display_putc
display_putc:
	svc SVCDisplayPutc
	bx lr

.thumb_func 
.global display_gotoxy
display_gotoxy:
	svc SVCDisplayGotoxy
	bx lr

.thumb_func 
.global display_cls
display_cls:
	svc SVCDisplayCls
	bx lr

.thumb_func 
.global display_numlines
display_numlines:
	/* prologue */
	push {lr}
	sub sp, #4				 /* uint32_t num_lines */

	mov r0, sp
	bl display_numlines_aux /* display_num_lines_aux( &num_lines ); */

	ldr r0, [sp]			 /* return (uint32_t)num_lines;  */

	/* epilogue */
	add sp, #4
	pop {pc}

display_numlines_aux:
	svc SVCDisplayNumLines
	bx lr

.thumb_func 
.global display_currline
display_currline:
	/* prologue */
	push {lr}
	sub sp, #4				 /* uint32_t curr_line */

	mov r0, sp
	bl display_currline_aux /* display_currline_aux( &curr_line ); */

	ldr r0, [sp]			 /* return (uint32_t)curr_line;  */

	/* epilogue */
	add sp, #4
	pop {pc}

display_currline_aux:
	svc SVCDisplayCurrLine
	bx lr

/* 
	LED
*/
.thumb_func 
.global led_write
led_write:
	svc SVCLedWrite
	bx lr

.thumb_func 
.global led_read
led_read:
	/* prologue */
	push {lr}
	sub sp, #4		/* tLedState state */

	mov r1, sp
	bl led_read_aux /* led_read_aux( led_num, &state ); */

	ldrb r0, [sp]	/* return (tLedState)state;  */

	/* epilogue */
	add sp, #4
	pop {pc}

led_read_aux:
	svc SVCLedRead
	bx lr


