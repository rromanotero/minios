/*
	A simple bootloader for ARMv8

  Started off from hte bootloader here:
      https://github.com/mmuszkow/NoOsBootstrap/tree/master/arm 

	Rafael R.O.
*/

.section ".boot"
.globl _bootloader
.align 2

_bootloader:
	@ in QEMU all of 4 ARM CPUs are started simultaniously         /* From                                                      */
	                                                               /* https://github.com/mmuszkow/NoOsBootstrap/tree/master/arm */
    @ by default. I don't know if this is the real hw behaviour,
    @ but here I jump to halt if CPU ID (stored in MPIDR
    @ register, first 2 bits) is not 0
    mrc p15, #0, r1, c0, c0, #5
    and r1, r1, #3
    cmp r1, #0
    bne halt

		// set vector address.
    ldr r0, =vector
    mcr P15, 0, r0, c12, c0, 0

		// set sp in irq mode.
		//(need this for IRQs)
		mrs r0, cpsr
		bic r1, r0, #0x1F
		orr r1, r1, #0x12
		msr cpsr_c,r1
		mov sp, #0x4000					/* I think somehow, this will
				 											 give diff Stacks to IRQs and he Kernel
															 See here (page 10):
															 	  ftp://ftp.intel.com/Pub/fpgaup/pub/Intel_Material/16.1/Tutorials/Using_GIC.pdf
														*/

		// set sp in svc mode.
		//(I think svc is supervisor mode)
		//See here:
		//    https://www.freertos.org/Using-FreeRTOS-on-Cortex-A-proprietary-interrupt-controller.html#modes-and-stacks
		msr cpsr_c, r0
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

		/* Set up Vector Table */
		ldr 		r0, =vector			/* The vector table */
		mov     r1, #0x0000			  	/* Where the Vector Table is*/
		ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
		stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
		ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
		stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

		/* Pass control to the Kernel */
		ldr r3, =main
		blx r3

halt:						/* Kernel should not return, but in case it does */
    wfe @ low-power mode
    b halt

/*
	         V E C T O R     T A B L E
*/

hang:
		b hang

/* IRQ Handler as per documentation*/
irq:
	 	push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
		bl c_irq_handler
		pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
		subs pc,lr,#4

.balign 32
vector:
		ldr pc, reset_handler
		ldr pc, undefined_handler
		ldr pc, swi_handler
		ldr pc, prefetch_handler
		ldr pc, data_handler
		ldr pc, unused_handler
		ldr pc, irq_handler
		ldr pc, fiq_handler

		reset_handler:      .word _bootloader
		undefined_handler:  .word hang
		swi_handler:        .word SVC_Handler
		prefetch_handler:   .word hang
		data_handler:       .word hang
		unused_handler:     .word hang
		irq_handler:        .word irq
		fiq_handler:        .word hang
