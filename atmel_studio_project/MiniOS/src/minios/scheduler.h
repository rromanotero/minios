/**
 * @file	scheduler.h
 * @author  
 * @version 
 *
 * @brief The (thread) scheduler's header file
 *
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define SCHEDULER_PROCESS_CREATE_FAILED		0
#define SCHEDULER_PROCESS_CREATE_SUCCESS	1

typedef uint32_t tProcessState;
enum tProcessState { ProcessStateReady = 0, ProcessStateRunning, ProcessStateNull  };	
		
void scheduler_init(void);
uint32_t scheduler_process_create( uint8_t*, uint8_t*, uint32_t* );

#endif /* SCHEDULER_H_ */