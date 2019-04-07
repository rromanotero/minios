
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

.globl IRQ_Handler
IRQ_Handler:
	ldr	r4, =irq_callback
	ldr r4, [r4]
	bx	r4

	.globl io_halt
	io_halt:
	    wfi
	    bx lr

	.globl hal_cpu_irq_enable
	hal_cpu_irq_enable:
	    cpsie i
	    bx lr

	.globl hal_cpu_irq_disable
	hal_cpu_irq_disable:
	    cpsid i
	    bx lr
