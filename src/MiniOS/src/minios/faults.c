/**
 * @file	faults.c
 * @author  
 * @version 
 *
 * @brief Faults module. This module handles everything related to CPU Fault Exceptions.
 *
 */
#include "system.h"
#include "HAL/hal.h"
#include "faults.h"
#include <stdio.h>

static void faults_app_entry_point(void);
static void faults_system_entry_point(void);
static uint32_t get_error_code(void);
static uint8_t* make_error_msg(uint32_t);
static uint8_t* code_to_msg(uint32_t code);

//Register offsets SCB SHCR
#define SCB_SHCSR_USGFAULTENA_OFFSET			18	/* pg. 223 ATSAM4SD32C Datasheet */
#define SCB_SHCSR_BUSFAULTENA_OFFSET			17
#define SCB_SHCSR_MEMFAULTENA_OFFSET			16

//Register offsets SCB CCR
#define SCB_CCR_DIV_0_TRP_OFFSET				4	/* pg. 217 ATSAM4SD32C Datasheet */
#define SCB_CCR_UNALIGN_TRP__OFFSET				3

//Register offsets SCB CFSR
#define	SCB_CFSR_DIVBYZERO						25 /* pg. 225 ATSAM4SD32C Datasheet */
#define	SCB_CFSR_UNALIGNED						24
#define	SCB_CFSR_NOCP							19
#define	SCB_CFSR_INVPC							28
#define	SCB_CFSR_INVSTATE						17
#define	SCB_CFSR_UNDEFINSTR						16
#define	SCB_CFSR_BFARVALID						15
#define	SCB_CFSR_STKERR							12
#define	SCB_CFSR_UNSTKERR						11
#define	SCB_CFSR_IMPRECISERR					10
#define	SCB_CFSR_PRECISERR						9
#define	SCB_CFSR_IBUSERR						8
#define	SCB_CFSR_MMARVALID						7
#define	SCB_CFSR_MSTKERR						4
#define	SCB_CFSR_MUNSTKERR						3
#define	SCB_CFSR_DACCVIOL						1
#define	SCB_CFSR_IACCVIOL						0


/**
*	Fault Init
*
*	Enables CPU Faults exceptions, and sets their respective 
*	callbacks (Usage Fault, Bus Fault, Mem Manager Fault, and Hard Fault ).
*
*	@param fault_type the fault type
*	@param callback the function that gets called on fault_type exception
*/
void faults_init(void){
	
	//Enables Usage, Bus, and Mem Manager Fault Exception
	//by setting USGFAULTENA, BUSFAULTENA, and MEMFAULTENA in
	//the SHCSR register part of the system control block (SCB) register
	//(sse pg. 223 ATSAM4SD32C)	
	uint32_t shcsr = 	hal_cpu_get_scb_shcsr();
	
	shcsr |= ((1 << SCB_SHCSR_USGFAULTENA_OFFSET)
				| (1 << SCB_SHCSR_BUSFAULTENA_OFFSET)
				| (1 << SCB_SHCSR_MEMFAULTENA_OFFSET) );
	
	hal_cpu_set_scb_shcsr( shcsr );

	//enables division by zero
	//by setting DIV_0_TRP in CCR register part of the SCB register
	//(sse pg. 217 ATSAM4SD32C)
	uint32_t ccr = 	hal_cpu_get_scb_ccr();
	
	ccr |= ((1 << SCB_CCR_DIV_0_TRP_OFFSET) /*| (1 << SCB_CCR_UNALIGN_TRP__OFFSET)*/); //Do not enable memory align traps
																					   //USB drivers seem to make some half-word memory accesses
	
	hal_cpu_set_scb_ccr( ccr );
	
	//sets callbacks
	hal_cpu_fault_register_callback( FaultApp, faults_app_entry_point );
	hal_cpu_fault_register_callback( FaultSystem, faults_system_entry_point );
}

/*
*	App Faults entry point
*
*   Execution comes here when a fault originated in an app.
*
*/
static void faults_app_entry_point(void){
	//If finding out the exact fault is necessary, read IPSR[0:8].
	//3 is HardFautl, 4 is MemManage, 5 Bus BusFault, and 6 is UsageFault
	system_panic( make_error_msg( get_error_code() ) );
}

/*
*	Kernel Faults entry point 
*
*   Execution comes here when a fault originated in the OS itself.
*
*/
static void faults_system_entry_point(void){
	//We are the cause of the fault 0.o, panic!
	system_panic( make_error_msg( get_error_code() ) );
}


/**
*	Get Error Code
*
*	Return the corresponding error code as per the Configurable Fault Status Register (CFSR). 
*	Error codes are one to one mapped to whatever bit they occupy within the CFSR. For example,
*	DIVBYZERO flag is bit 25 in CFSR, so the code for division by zero is 25
*	See pg 223. in ATSAM4SD32C Documentation
*
*/
static uint32_t get_error_code(void){
	
	uint32_t fault_status = hal_cpu_get_scb_cfsr();

	for(int i=0; i<31; i++){
		if( (fault_status & (1<<i)) )
			return i;
	}
}

/**
*	Make Error message
*
*	Given an error code it return a string describing a fatal error,
*	the error code, and a description of the code. Thsui
*
*	@param code the error code
*	
*	@return the string with the error message 
*/
static uint8_t error_msg[SYS_PANIC_MSG_MAX_LENGTH]; //the panic error message to be printed 

static uint8_t* make_error_msg(uint32_t code){
	
	sprintf(error_msg, "Code %d (%s)", code, code_to_msg(code));
	
	return error_msg;
}

/**
*	Code to msg
*
*	Creates a custom message for a given error code (see pg 223 documentation)
*
*	@param code the error code
*	
*	@return	a pointer to the message
*/
static uint8_t* code_to_msg(uint32_t code){
	
	switch(code){
		case 0: return "Instruction Access";
		case 1: return "Data Access";
		case 3: return "Mem mgr on Unstacking";
		case 4: return "Mem mgr on Stacking";
		case 7: return "Mem mgr FAR Valid FA Register Valid";
		case 8: return "Instruction Bus";
		case 9: return "Precise Data Bus";
		case 10: return "Imprecise Data Bus";
		case 11: return "Bus Fault On Unstacking";
		case 12: return "Bus Fault On Stacking";
		case 15: return "Bus Fault AR Valid";
		case 16: return "Undefined Instruction";
		case 17: return "Invalid State";
		case 18: return " Invalid PC Load";
		case 19: return "No Coprocessor";
		case 24: return "Unaligned Access";
		case 25: return "Divide by Zero";
		default: return "Wrong Error Code";
	}
}