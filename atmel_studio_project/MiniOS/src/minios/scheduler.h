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

#include "system.h"

#define SCHEDULER_PROCESS_CREATE_FAILED		0
#define SCHEDULER_PROCESS_CREATE_SUCCESS	1
#define SCHEDULER_MAX_NUM_PROCESSES			SYS_MAX_NUM_OF_PROCESSES

typedef uint32_t tProcessState;
enum tProcessState { ProcessStateReady = 0, ProcessStateRunning, ProcessStateDead, ProcessStateNull  };	

typedef struct{
	uint8_t* name;
	uint32_t* sp;
	tProcessState state;
}tMiniProcess;

typedef struct{
	tMiniProcess list[SCHEDULER_MAX_NUM_PROCESSES];
	uint32_t count;
}tProcessList;
		
void scheduler_init(void);
uint32_t scheduler_process_create( uint8_t*, uint8_t*, uint32_t* );
uint32_t scheduler_thread_create( uint8_t*, uint8_t*, uint32_t );
void scheduler_process_current_stop();

#endif /* SCHEDULER_H_ */