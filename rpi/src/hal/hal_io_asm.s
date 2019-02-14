/*
* HAL IO (assembly)
*
* Adapted From:
*     https://github.com/mmuszkow/NoOsBootstrap/tree/master/arm
*/

.section .text
.align 2

.equ INIT_FAILED, 1
.equ INIT_SUCCESS, 0

/*
*   HAL IO Video Init
*
*   void _hal_io_video_init(void)
*/
.globl _hal_io_video_init
_hal_io_video_init:
	push {r4-r9, lr}

	mov r4, #7
	@ set Video Controller resolution to 640x480x16bit
	@ 16-bit, cause the 8-bit depth needs a palette
	@ and I'm too lazy to set it up
	ldr r1, =vc_set_res
	bl  mb0_c8_write
	bl  mb0_c8_read
	tst r0, #0x80000000
	beq _video_init_failed

	@ get VC framebuffer address
	ldr r1, =vc_alloc_fb
	bl  mb0_c8_write
	bl  mb0_c8_read
	tst r0, #0x80000008
	beq _video_init_failed

	@ check if the address is correct
	ldr r0, [r1, #20]
	cmp r0, #0
	beq _video_init_failed

	@Store framebuffer pointer
	ldr r4, =frame_buffer_ptr
	str r0, [r4]

	mov r0, #INIT_SUCCESS
  b	_video_init_success

_video_init_failed:
	mov r0, #INIT_FAILED
_video_init_success:
	pop {r4-r9, pc}


/*
*   HAL IO Video Put Pixel
*
*   void _hal_io_video_put_pixel_raw(uint32_t raw_linear_position, Color)
*/
.globl _hal_io_video_put_pixel_raw
_hal_io_video_put_pixel_raw:
	fb    		.req r4
	position 	.req r0
	color 		.req r1

	push {r4-r6, lr}

	@Set up the frame buffer ptr
	ldr r5, =frame_buffer_ptr
	ldr fb, [r5]

	@Set up position
	add fb, position

	@Draw Pixel
	str color, [fb, #0]
	str color, [fb, #2]   @This is so that letters look BOLD

	pop {r4-r6, pc}



@@@@@@@@@@@@@@@ VC @@@@@@@@@@@@@@

.equ MBOX0, 0x3f00b880

@ writes to mailbox #0, channel 8
@ r1 - message
mb0_c8_write:
    message .req r1
    mailbox .req r3
    status  .req r6

    ldr mailbox, =MBOX0
.mb0_full:
    ldr status, [mailbox, #0x18]
    tst status, #0x80000000  @ mailbox full flag
    bne .mb0_full
    add message, #8          @ channel 8
    str message, [mailbox, #0x20] @ write addr
    sub message, #8

    .unreq mailbox
    .unreq message
    .unreq status
    mov pc, lr

@ reads from mailbox #0, channel 8
@ r1 - message
@ returns status in r0
mb0_c8_read:
    message .req r1
    mailbox .req r6
    status  .req r3
    value   .req r4

    ldr mailbox, =MBOX0
.mb0_empty:
    ldr status, [mailbox, #0x18]
    tst status, #0x40000000  @ mailbox empty flag
    bne .mb0_empty

    ldr value, [mailbox] @ check if the message channel is 8
    and r0, value, #0xf
    teq r0, #8
    bne .mb0_empty

    ldr r0, [message, #4]
    .unreq message
    .unreq mailbox
    .unreq status
    .unreq value
    mov pc, lr

@ raspi mailbox requests, must be padded to 16 bytes
.align 4
vc_set_res:  .word 80, 0                      @ total size, code (0=req)
             .word 0x00048003, 8, 8, 640, 480 @ set physical size (640x480)
             .word 0x00048004, 8, 8, 640, 480 @ set virtual size (640x480)
             .word 0x00048005, 4, 4, 16       @ set depth (16-bit)
             .word 0, 0, 0, 0                 @ end tag & padding

vc_alloc_fb: .word 32, 0                      @ total size, code (0=req)
             .word 0x00040001, 8, 4, 16, 0    @ allocate framebuffer
             .word 0                          @ end tag & padding



frame_buffer_ptr:  .word 0
