/*
	A simple bootloader for ARMv8

	Rafael R.O.
*/

.section ".boot"
.globl _bootloader
.align 2

_bootloader:
	@ in QEMU all of 4 ARM CPUs are started simultaniously         /* From                                                      */
	                                                               /* https://github.com/mmuszkow/NoOsBootstrap/tree/master/arm */
    @ by default. I don't know if this is the real hw behaviour,   /*                                                            */
    @ but here I jump to halt if CPU ID (stored in MPIDR           /* I have no idea what it does, but it fixed some flickering  */
    @ register, first 2 bits) is not 0
    mrc p15, #0, r1, c0, c0, #5
    and r1, r1, #3
    cmp r1, #2
    bne halt

	ldr r4, =__stack_end 			/* Set up SP */
	mov sp, r4

	ldr r4, =__bss_start__			/* Fill BSS with Zeroes */
	ldr r9, =__bss_end__
	mov r5, #0

next_word:
	cmp r4, r9
	bge end_bss_init
	stmia r4!, {r5}
	b next_word

end_bss_init:

	ldr r3, =main			/* Pass control to the Kernel */
	blx r3

halt:						/* Kernel should not return, but in case it does */
    wfe @ low-power mode
    b halt
