
.section .text
.align 2

//SVC Handler
//(no prologue and no epilogue to avoid
// changing the offsets of the stacked frame on SVC interrupt)

.globl SVC_Handler
SVC_Handler:
	ldr	r4, =svc_callback
	ldr r4, [r4]
	bx	r4

.globl IRQS_Handler
IRQS_Handler:
	ldr	r4, =irqs_callback
	ldr r4, [r4]
	bx	r4

	.globl io_halt
	io_halt:
	    wfi
	    bx lr

	.globl enable_irq
	enable_irq:
	    cpsie i
	    bx lr

	.globl disable_irq
	disable_irq:
	    cpsid i
	    bx lr
