/**
 * @file	hal.h
 * @author  
 * @version 
 *
 * @brief Header file for the IO HAL
 *
 */

#ifndef HAL_H_
#define HAL_H_

#include <stdint-gcc.h>	//defs for size-specific primitive data types
#include <stdbool.h>	//defs for true and false

typedef uint32_t tLedNum;		/**< Led number type */
typedef bool tLedState;			/**< Led state type */
typedef uint32_t tButtonNum;	/**< Button number type */
typedef uint32_t tButtonState;	/**< Button state type */
typedef uint32_t tSensorId;		/**< Sensor ID type */
typedef uint32_t tSerialId;		/**< Serial ID type */
typedef uint32_t tNVMemId;		/**< Non-volatile Memory ID type */
typedef uint32_t tMemRegionId;	/**< Memory Region ID type */
typedef uint32_t tFaultOrigin;	/**< Fault Origin type */

enum tLedState	{ LedOff = false, LedOn };								/**< enumeration for LED states */
enum tLedNum	{ Led0 = 0, Led1, Led2, Led3, Led4 };					/**< enumeration for LEDs in the board */
enum tButtonNum	{ Button0 = 0, Button1, Button2, Button3, Button4 };	/**< enumeration for buttons in the board*/
enum tButtonState { ButtonUnpressed = false, ButtonPressed };				/**< enumeration for Button states */
enum tSensorId	{ SensorLight = 0, SensorTemp };						/**< enumeration for supported sensors */
enum tSerialId	{ SerialA = 0, SerialB };								/**< enumeration for supported serial interfaces */
enum tNVMemId	{ NVMemSDCard = 0, NVMemSDCardFAT, NVMemFlash };		/**< enumeration for non-volatile memories */	
enum tMemRegionId { MemRegSystem = 0, MemRegApp,
					MemRegSystemStack, MemRegUserStack };				/**< enumeration for memory regions */
enum tFaultOrigin { FaultApp = 0, FaultSystem };						/**< enumeration for Fault Origin types */

#define MEM_FAT_MAX_FNAME_LENGTH	30	/**< max file name size for the FAT file system */

/**
* A structure to represent time
*/
typedef struct{
	uint32_t seconds; /**< seconds */
	uint32_t minutes; /**< minutes */
	uint32_t hours;   /**< hours */
	uint32_t day;     /**< day */
	uint32_t month;   /**< month */
	uint32_t year;    /**< year */
}tTime;

/**
* A structure to represent memory regions
*/
typedef struct{
	tMemRegionId	id;
	uint8_t*		base;		/**< a pointer to the beginning of the region */
	uint32_t		size;		/**< size in bytes */
}tMemRegion;

//  --------------  INIT  ---------------
//  -------------------------------------
void hal_io_init( void );
void hal_cpu_init( void );
void hal_nvmem_init(void);
void hal_memreg_init( void );

//  --------------  IO  -----------------
//  -------------------------------------

//Millisecond Timer
void hal_io_mtimer_start( uint32_t );
void hal_io_mtimer_start_int( uint32_t, void(*)(void) );
void hal_io_mtimer_stop( void );
uint32_t hal_io_mtimer_read( void );

//Sensor
void hal_io_sensor_start_poll( tSensorId );
void hal_io_sensor_start_int( tSensorId, void(*)(void) );
uint32_t hal_io_sensor_read( tSensorId );

//Display
void hal_io_display_start( void );
void hal_io_display_putc( uint8_t );
void hal_io_display_cls( void );
void hal_io_display_gotoxy( uint32_t, uint32_t );
uint32_t hal_io_display_numlines( void );
uint32_t hal_io_display_currline( void );

//LED
void hal_io_led_start( void );
void hal_io_led_write( tLedNum, tLedState );
tLedState hal_io_led_read( tLedNum );

//Clock
void hal_io_clock_start( void );
void hal_io_clock_write( tTime* );
void hal_io_clock_read( tTime* );

//Buttons
void hal_io_button_startall_int( void(*)(tButtonNum) );
void hal_io_button_startall_poll( void );
tButtonState hal_io_button_read( tButtonNum );

//Serial
void hal_io_serial_start_poll( tSerialId, uint32_t );
void hal_io_serial_start_int( tSerialId, uint32_t, void(*)(uint32_t) );
void hal_io_serial_putc( tSerialId, uint8_t );
uint8_t hal_io_serial_getc( tSerialId );

//System Timer
void hal_cpu_systimer_start(uint32_t, void(*)(void));
void hal_cpu_systimer_stop(void);
void hal_cpu_systimer_reestart(void);

//  --------------  CPU  ----------------
//  -------------------------------------

//Scheduler
void hal_cpu_return_exception_user_mode();

//PendSV
void hal_cpu_lowpty_softint_trigger(void);
void hal_cpu_lowpty_softint_register_callback(void(*)(void));

//System Timer
void hal_cpu_systimer_stop(void);
void hal_cpu_systimer_reestart(void);
void hal_cpu_systimer_start(uint32_t, void(*)(void));

//Scheduler
void hal_cpu_save_context(void);
void hal_cpu_restore_context(void);
void hal_cpu_sleep(void);

//Faults
void hal_cpu_fault_register_callback( tFaultOrigin, void(*)(void) );

//CPU Registers
uint32_t hal_cpu_speed_in_hz( void );
void hal_cpu_set_unprivileged( void );
void hal_cpu_set_psp_active( void );
void hal_cpu_set_psp( uint32_t );
uint32_t hal_cpu_get_psp( void ) ;
void hal_cpu_set_scb_shcsr(uint32_t);
uint32_t hal_cpu_get_scb_shcsr(void);
void hal_cpu_set_scb_ccr(uint32_t);
uint32_t hal_cpu_get_scb_ccr(void);
uint32_t hal_cpu_get_scb_cfsr(void);

//Delays
void hal_cpu_delay(uint32_t);

//SVC
void hal_cpu_svc_start( void(*)(void) );

//  -------  NON-VOLATILE MEMORY  -------
//  -------------------------------------
bool hal_nvmem_start( tNVMemId );
double hal_nvmem_capacity( tNVMemId );
uint32_t hal_nvmem_fat_file_size( uint8_t* );
uint32_t hal_nvmem_fat_file_read( uint8_t*, uint8_t* , uint32_t );
bool hal_nvmem_fat_file_write( uint8_t*, uint8_t*, uint32_t );
uint32_t hal_nvmem_fat_file_list( uint8_t list[][MEM_FAT_MAX_FNAME_LENGTH+1], uint32_t );


//  ---------  MEMORY REGIONS  ----------
//  -------------------------------------
void hal_memreg_read( tMemRegionId, tMemRegion* );


void thread_create( void (*)(void*), uint8_t*,  uint32_t, void* );

#endif /* HAL_H_ */