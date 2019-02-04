
/*
 * scheduler_asm.s
 *
 * Assembly part of the scheduler
 *
 */ 

 /*	Puts the processor to sleep. executes in user mode  */
.thumb_func 
.global idle_process_thread
idle_process_thread:  
		//ldr r3, =hal_cpu_sleep    <--- Debugger shows ldr is not getting the right address
		//blx r3					
		wfi					/* while(true) sleep();*/
		b idle_process_thread

/*	Deletes the current process/thread. this function will be 
	accessed in user mode when a thread "returns" hence the syscall  */
.thumb_func 
.global process_thread_delete
process_thread_delete:
		svc 29 /* thread_stop system call see syscalls.c */
		b .		/* execution won't get here */
