/**
 * @file	hal_asm.s
 * @author  
 * @version 
 *
 * @brief Assembly for the CPU Part of the Hardware Abstraction Layer 
 *
 */

 .equ SCB_SHCSR,	0xE000ED24	/* pg 217 documentation*/
 .equ SCB_CCR,		0xE000ED14
 .equ SCB_CFSR,		0xE000ED28	
 
 .equ USER_MODE_EXEC_VALUE, 0xFFFFFFFD 

 .syntax unified
.section .text



/**
*
*	Sleep
*
*	Executes the instruction that puts the CPU
*	to sleep
*/
.thumb_func 
.global hal_cpu_sleep
hal_cpu_sleep:
     wfi
     bx lr

/**
*	__attribute__((naked)) static void hal_cpu_return_from_exception_user_mode(){
*
*	Return from exception in user mode
*
*	Returns from an exception, specifying an EXEC_VALUE 
*	of 0xFFFFFFFD i.e. specifyin user mode and PSP as active SP 
*/

.thumb_func 
.global hal_cpu_return_exception_user_mode
hal_cpu_return_exception_user_mode:
	ldr pc, =USER_MODE_EXEC_VALUE

/**
*	void hal_cpu_save_context(void)
*
*	CPU Save Context
*
*	Pushes onto the stack the registers
*	R4-R11 that ought to be saved "in software"
*	during context switch
*
*/
.thumb_func 
.global hal_cpu_save_context
hal_cpu_save_context:
     mrs r0, psp
	 stmfd r0!, {r4-r11}
	 msr psp, r0

	 bx lr

/**
*	void hal_cpu_restore_context(void)
*
*	CPU Restore Context
*
*	Pops from the stack the registers
*	R4-R11 that ought to be restored "in software"
*	during context switch
*
*/
.thumb_func 
.global hal_cpu_restore_context
hal_cpu_restore_context:
     mrs r0, psp
	 ldmfd r0!, {r4-r11}
	 msr psp, r0

	 bx lr

//
//PendSV Handler
//(no prologue and no epilogue)
//
.thumb_func 
.global PendSV_Handler
PendSV_Handler:
	ldr	r3, =pendsv_callback
	ldr r3, [r3]
	bx	r3

//
//Systick Handler
//(no prologue and no epilogue)
//
.thumb_func 
.global SysTick_Handler
SysTick_Handler:
	ldr	r3, =systick_callback
	ldr r3, [r3]
	bx	r3

//BusFault Handler 
//(no prologue and no epilogue to avoid
// changing the offsets of the stacked frame on SVC interrupt)
.thumb_func 
.global BusFault_Handler
BusFault_Handler:
	b faults_goto_right_callback

//UsageFault Handler 
//(no prologue and no epilogue to avoid
// changing the offsets of the stacked frame on SVC interrupt)
.thumb_func 
.global UsageFault_Handler
UsageFault_Handler:
	b faults_goto_right_callback

// MemManage Handler 
//(no prologue and no epilogue to avoid
// changing the offsets of the stacked frame on SVC interrupt)
.thumb_func 
.global MemManage_Handler
MemManage_Handler:
	b faults_goto_right_callback

//HardFault Handler 
//(no prologue and no epilogue to avoid
// changing the offsets of the stacked frame on SVC interrupt)
.thumb_func 
.global HardFault_Handler
HardFault_Handler:
	b faults_goto_right_callback
	

faults_goto_right_callback:
// Bit 2 in EXC_RETURN (placed in the LR on exception entry) tells 
// which stack to use on exception return 
	tst lr, 0b100
	beq faults_else					/* if ( exc_return & 0b0100 ) */
	ldr	r0, =fault_app_callback		/*    temp = app_callback;     */
	b faults_end					    /*							   */
faults_else:							/* else						   */
	ldr	r0, =fault_system_callback	/*    temp = system_callback;  */					
faults_end:
	ldr r0, [r0]									
	bx	r0						    /* temp(); */	



/**
*	void hal_cpu_set_scb_shcsr(uint32_t) 
*
*	Sets SHCSR
*
*	Sets the System Handler Control and State 
*	Register (SHCSR) from the System Control Block (SCB)
*
*/
.thumb_func 
.global hal_cpu_set_scb_shcsr
hal_cpu_set_scb_shcsr:
     ldr r1, =SCB_SHCSR
	 str r0, [r1]

     bx lr


/**
*	uint32_t hal_cpu_get_scb_shcsr(void) 
*
*	Gets SHCSR
*
*	Gets the System Handler Control and State 
*	Register (SHCSR) from the System Control Block (SCB)
*
*/
.thumb_func 
.global hal_cpu_get_scb_shcsr
hal_cpu_get_scb_shcsr:
     ldr r1, =SCB_SHCSR
	 ldr r0, [r1]

     bx lr



/**
*	void hal_cpu_set_scb_ccr(uint32_t) 
*
*	Sets CCR
*
*	Sets the Configuration and Control
*	Register (CCR) from the System Control Block (SCB)
*
*/
.thumb_func 
.global hal_cpu_set_scb_ccr
hal_cpu_set_scb_ccr:
     ldr r1, =SCB_CCR
	 str r0, [r1]

     bx lr


/**
*	uint32_t hal_cpu_set_scb_ccr(void) 
*
*	Gets CCR
*
*	Gets the Configuration and Control
*	Register (CCR) from the System Control Block (SCB)
*
*/
.thumb_func 
.global hal_cpu_get_scb_ccr
hal_cpu_get_scb_ccr:
     ldr r1, =SCB_CCR
	 ldr r0, [r1]

     bx lr



/**
*	void hal_cpu_get_scb_cfsr(void) 
*
*	Gets the Configurable Fault Status Register (CFSR)
*	from the System Control Block (SCB)
*
*	Makes the CPU sleep by executing the WFI
*	instruction
*/
.thumb_func 
.global hal_cpu_get_scb_cfsr
hal_cpu_get_scb_cfsr:
     ldr r1, =SCB_CFSR
	 ldr r0, [r1]

     bx lr


//SVC Handler
//(no prologue and no epilogue to avoid
// changing the offsets of the stacked frame on SVC interrupt)
.thumb_func 
.global SVC_Handler
SVC_Handler:
	ldr	r3, =svc_callback
	ldr r3, [r3]
	bx	r3


/**
*	uint32_t hal_cpu_get_psp(void) 
*
*	Gets the PSP
*
*	Returns the process stack pointer
*/
.thumb_func 
.global hal_cpu_get_psp
hal_cpu_get_psp:
	mrs	r0, psp  //return psp
	bx lr 

/**
*	void hal_cpu_set_unprivileged(void) 
*
*	CPU Set unprivileged
*
*	Set the CPU as unprivileged (when in thread mode)s
*/
.thumb_func 
.global hal_cpu_set_unprivileged
hal_cpu_set_unprivileged:
	mrs r3, control
	orr	r3, r3, #1
	msr control, r3 /* control |= 1 */
	isb

	bx lr

/**
*	void hal_cpu_psp_active(void) 
*
*	CPU Set PSP active
*
*	Sets the Process Stack Pointer as active (when in thread mode)
*/
.thumb_func 
.global hal_cpu_set_psp_active
hal_cpu_set_psp_active:
	mrs r3, control
	orr	r3, r3, #2
	msr control, r3 /* control |= 2 */
	isb

	bx lr
/**
*	void hal_cpu_set_psp(uint32_t) 
*
*	Sets the PSP
*
*	Sets the Process Stack Pointer value
*/
.thumb_func 
.global hal_cpu_set_psp
hal_cpu_set_psp:
     msr psp, r0

     bx lr

