/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++}
{																			}
{       Filename: rpi-smartstart.c											}
{       Copyright(c): Leon de Boer(LdB) 2017								}
{       Version: 2.02														}
{																			}
{***************[ THIS CODE IS FREEWARE UNDER CC Attribution]***************}
{																            }
{     This sourcecode is released for the purpose to promote programming    }
{  on the Raspberry Pi. You may redistribute it and/or modify with the      }
{  following disclaimer and condition.                                      }
{																            }
{      The SOURCE CODE is distributed "AS IS" WITHOUT WARRANTIES AS TO      }
{   PERFORMANCE OF MERCHANTABILITY WHETHER EXPRESSED OR IMPLIED.            }
{   Redistributions of source code must retain the copyright notices to     }
{   maintain the author credit (attribution) .								}
{																			}
{***************************************************************************}
{                                                                           }
{      This code provides a 32bit or 64bit C wrapper around the assembler   }
{  stub code. In AARCH32 that file is SmartStart32.S, while in AARCH64 the  }
{  file is SmartStart64.S.													}
{	   There file also provides access to the basic hardware of the PI.     }
{  It is also the easy point to insert a couple of important very useful    }
{  Macros that make C development much easier.		        				}
{																            }
{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <stdbool.h>							// Needed for bool and true/false
#include <stdint.h>								// Needed for uint8_t, uint32_t, uint64_t etc
#include <stdarg.h>								// Needed for variadic arguments
#include <string.h>								// Needed for strlen
#include "rpi-SmartStart.h"						// This units header
#include "../../hal/hal.h"

/***************************************************************************}
{       PRIVATE INTERNAL RASPBERRY PI REGISTER STRUCTURE DEFINITIONS        }
****************************************************************************/

/*--------------------------------------------------------------------------}
{    RASPBERRY PI GPIO HARDWARE REGISTERS - BCM2835.PDF Manual Section 6	}
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) GPIORegisters {
	volatile uint32_t GPFSEL[6];									// 0x00  GPFSEL0 - GPFSEL5
	uint32_t reserved1;												// 0x18  reserved
	volatile uint32_t GPSET[2];										// 0x1C  GPSET0 - GPSET1;
	uint32_t reserved2;												// 0x24  reserved
	volatile uint32_t GPCLR[2];										// 0x28  GPCLR0 - GPCLR1
	uint32_t reserved3;												// 0x30  reserved
	const volatile uint32_t GPLEV[2];								// 0x34  GPLEV0 - GPLEV1   ** Read only hence const
	uint32_t reserved4;												// 0x3C  reserved
	volatile uint32_t GPEDS[2];										// 0x40  GPEDS0 - GPEDS1
	uint32_t reserved5;												// 0x48  reserved
	volatile uint32_t GPREN[2];										// 0x4C  GPREN0 - GPREN1;
	uint32_t reserved6;												// 0x54  reserved
	volatile uint32_t GPFEN[2];										// 0x58  GPFEN0 - GPFEN1;
	uint32_t reserved7;												// 0x60  reserved
	volatile uint32_t GPHEN[2];										// 0x64  GPHEN0 - GPHEN1;
	uint32_t reserved8;												// 0x6c  reserved
	volatile uint32_t GPLEN[2];										// 0x70  GPLEN0 - GPLEN1;
	uint32_t reserved9;												// 0x78  reserved
	volatile uint32_t GPAREN[2];									// 0x7C  GPAREN0 - GPAREN1;
	uint32_t reserved10;											// 0x84  reserved
	volatile uint32_t GPAFEN[2]; 									// 0x88  GPAFEN0 - GPAFEN1;
	uint32_t reserved11;											// 0x90  reserved
	volatile uint32_t GPPUD; 										// 0x94  GPPUD
	volatile uint32_t GPPUDCLK[2]; 									// 0x98  GPPUDCLK0 - GPPUDCLK1;
};

/*--------------------------------------------------------------------------}
{  RASPBERRY PI SYSTEM TIMER HARDWARE REGISTERS - BCM2835 Manual Section 12	}
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) SystemTimerRegisters {
	volatile uint32_t ControlStatus;								// 0x00
	volatile uint32_t TimerLo;										// 0x04
	volatile uint32_t TimerHi;										// 0x08
	volatile uint32_t Compare0;										// 0x0C
	volatile uint32_t Compare1;										// 0x10
	volatile uint32_t Compare2;										// 0x14
	volatile uint32_t Compare3;										// 0x18
};

/*--------------------------------------------------------------------------}
{	   TIMER_CONTROL REGISTER BCM2835 ARM Peripheral manual page 197		}
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) TimerControlReg {
	union {
		struct __attribute__((__packed__, aligned(1))) {
			unsigned unused : 1;									// @0 Unused bit
			volatile unsigned Counter32Bit : 1;						// @1 Counter32 bit (16bit if false)
			volatile TIMER_PRESCALE Prescale : 2;					// @2-3 Prescale
			unsigned unused1 : 1;									// @4 Unused bit
			volatile unsigned TimerIrqEnable : 1;					// @5 Timer irq enable
			unsigned unused2 : 1;									// @6 Unused bit
			volatile unsigned TimerEnable : 1;						// @7 Timer enable
			unsigned reserved : 24;									// @8-31 reserved
		};
		uint32_t Raw32;												// Union to access all 32 bits as a uint32_t
	};
};

/*--------------------------------------------------------------------------}
{   RASPBERRY PI ARM TIMER HARDWARE REGISTERS - BCM2835 Manual Section 14	}
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) ArmTimerRegisters {
	volatile uint32_t Load;											// 0x00
	const volatile uint32_t Value;									// 0x04  ** Read only hence const
	struct TimerControlReg Control;									// 0x08
	volatile uint32_t Clear;										// 0x0C
	const volatile uint32_t RawIRQ;									// 0x10  ** Read only hence const
	const volatile uint32_t MaskedIRQ;								// 0x14  ** Read only hence const
	volatile uint32_t Reload;										// 0x18
};

/*--------------------------------------------------------------------------}
{   IRQ BASIC PENDING REGISTER - BCM2835.PDF Manual Section 7 page 113/114  }
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) IrqBasicPendingReg {
	union {
		struct __attribute__((__packed__, aligned(1))) {
			const volatile unsigned Timer_IRQ_pending : 1;			// @0 Timer Irq pending
			const volatile unsigned Mailbox_IRQ_pending : 1;		// @1 Mailbox Irq pending
			const volatile unsigned Doorbell0_IRQ_pending : 1;		// @2 Arm Doorbell0 Irq pending
			const volatile unsigned Doorbell1_IRQ_pending : 1;		// @3 Arm Doorbell0 Irq pending
			const volatile unsigned GPU0_halted_IRQ_pending : 1;	// @4 GPU0 halted IRQ pending
			const volatile unsigned GPU1_halted_IRQ_pending : 1;	// @5 GPU1 halted IRQ pending
			const volatile unsigned Illegal_access_type1_pending : 1; // @6 Illegal access type 1 IRQ pending
			const volatile unsigned Illegal_access_type0_pending : 1; // @7 Illegal access type 0 IRQ pending
			const volatile unsigned Bits_set_in_pending_register_1 : 1;	// @8 One or more bits set in pending register 1
			const volatile unsigned Bits_set_in_pending_register_2 : 1;	// @9 One or more bits set in pending register 2
			const volatile unsigned GPU_IRQ_7_pending : 1;			// @10 GPU irq 7 pending
			const volatile unsigned GPU_IRQ_9_pending : 1;			// @11 GPU irq 9 pending
			const volatile unsigned GPU_IRQ_10_pending : 1;			// @12 GPU irq 10 pending
			const volatile unsigned GPU_IRQ_18_pending : 1;			// @13 GPU irq 18 pending
			const volatile unsigned GPU_IRQ_19_pending : 1;			// @14 GPU irq 19 pending
			const volatile unsigned GPU_IRQ_53_pending : 1;			// @15 GPU irq 53 pending
			const volatile unsigned GPU_IRQ_54_pending : 1;			// @16 GPU irq 54 pending
			const volatile unsigned GPU_IRQ_55_pending : 1;			// @17 GPU irq 55 pending
			const volatile unsigned GPU_IRQ_56_pending : 1;			// @18 GPU irq 56 pending
			const volatile unsigned GPU_IRQ_57_pending : 1;			// @19 GPU irq 57 pending
			const volatile unsigned GPU_IRQ_62_pending : 1;			// @20 GPU irq 62 pending
			unsigned reserved : 10;									// @21-31 reserved
		};
		const volatile uint32_t Raw32;								// Union to access all 32 bits as a uint32_t
	};
};

/*--------------------------------------------------------------------------}
{	   FIQ CONTROL REGISTER BCM2835.PDF ARM Peripheral manual page 116		}
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) FiqControlReg {
	union {
		struct __attribute__((__packed__, aligned(1))) {
			volatile unsigned SelectFIQSource : 7;					// @0-6 Select FIQ source
			volatile unsigned EnableFIQ : 1;						// @7 enable FIQ
			unsigned reserved : 24;									// @8-31 reserved
		};
		volatile uint32_t Raw32;									// Union to access all 32 bits as a uint32_t
	};
};

/*--------------------------------------------------------------------------}
{	 ENABLE BASIC IRQ REGISTER BCM2835 ARM Peripheral manual page 117		}
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) EnableBasicIrqReg {
	union {
		struct __attribute__((__packed__, aligned(1))) {
			volatile unsigned Enable_Timer_IRQ : 1;					// @0 Timer Irq enable
			volatile unsigned Enable_Mailbox_IRQ : 1;				// @1 Mailbox Irq enable
			volatile unsigned Enable_Doorbell0_IRQ : 1;				// @2 Arm Doorbell0 Irq enable
			volatile unsigned Enable_Doorbell1_IRQ : 1;				// @3 Arm Doorbell0 Irq enable
			volatile unsigned Enable_GPU0_halted_IRQ : 1;			// @4 GPU0 halted IRQ enable
			volatile unsigned Enable_GPU1_halted_IRQ : 1;			// @5 GPU1 halted IRQ enable
			volatile unsigned Enable_Illegal_access_type1 : 1;		// @6 Illegal access type 1 IRQ enable
			volatile unsigned Enable_Illegal_access_type0 : 1;		// @7 Illegal access type 0 IRQ enable
			unsigned reserved : 24;									// @8-31 reserved
		};
		volatile uint32_t Raw32;									// Union to access all 32 bits as a uint32_t
	};
};

/*--------------------------------------------------------------------------}
{	DISABLE BASIC IRQ REGISTER BCM2835 ARM Peripheral manual page 117		}
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) DisableBasicIrqReg {
	union {
		struct __attribute__((__packed__, aligned(1))) {
			volatile unsigned Disable_Timer_IRQ : 1;				// @0 Timer Irq disable
			volatile unsigned Disable_Mailbox_IRQ : 1;				// @1 Mailbox Irq disable
			volatile unsigned Disable_Doorbell0_IRQ : 1;			// @2 Arm Doorbell0 Irq disable
			volatile unsigned Disable_Doorbell1_IRQ : 1;			// @3 Arm Doorbell0 Irq disable
			volatile unsigned Disable_GPU0_halted_IRQ : 1;			// @4 GPU0 halted IRQ disable
			volatile unsigned Disable_GPU1_halted_IRQ : 1;			// @5 GPU1 halted IRQ disable
			volatile unsigned Disable_Illegal_access_type1 : 1;		// @6 Illegal access type 1 IRQ disable
			volatile unsigned Disable_Illegal_access_type0 : 1;		// @7 Illegal access type 0 IRQ disable
			unsigned reserved : 24;									// @8-31 reserved
		};
		volatile uint32_t Raw32;									// Union to access all 32 bits as a uint32_t
	};
};

/*--------------------------------------------------------------------------}
{	   RASPBERRY PI IRQ HARDWARE REGISTERS - BCM2835 Manual Section 7	    }
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) IrqControlRegisters {
	const volatile struct IrqBasicPendingReg IRQBasicPending;		// 0x200   ** Read only hence const
	volatile uint32_t IRQPending1;									// 0x204
	volatile uint32_t IRQPending2;									// 0x208
	volatile struct FiqControlReg FIQControl;						// 0x20C
	volatile uint32_t EnableIRQs1;									// 0x210
	volatile uint32_t EnableIRQs2;									// 0x214
	volatile struct EnableBasicIrqReg EnableBasicIRQs;				// 0x218
	volatile uint32_t DisableIRQs1;									// 0x21C
	volatile uint32_t DisableIRQs2;									// 0x220
	volatile struct DisableBasicIrqReg DisableBasicIRQs;			// 0x224
};

/*--------------------------------------------------------------------------}
;{               RASPBERRY PI MAILBOX HARRDWARE REGISTERS					}
;{-------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) MailBoxRegisters {
	const volatile uint32_t Read0;									// 0x00         Read data from VC to ARM
	uint32_t Unused[3];												// 0x04-0x0F
	volatile uint32_t Peek0;										// 0x10
	volatile uint32_t Sender0;										// 0x14
	volatile uint32_t Status0;										// 0x18         Status of VC to ARM
	volatile uint32_t Config0;										// 0x1C
	volatile uint32_t Write1;										// 0x20         Write data from ARM to VC
	uint32_t Unused2[3];											// 0x24-0x2F
	volatile uint32_t Peek1;										// 0x30
	volatile uint32_t Sender1;										// 0x34
	volatile uint32_t Status1;										// 0x38         Status of ARM to VC
	volatile uint32_t Config1;										// 0x3C
};

/***************************************************************************}
{        PRIVATE INTERNAL RASPBERRY PI REGISTER STRUCTURE CHECKS            }
****************************************************************************/

/*--------------------------------------------------------------------------}
{					 CODE TYPE STRUCTURE COMPILE TIME CHECKS	            }
{--------------------------------------------------------------------------*/
/* If you have never seen compile time assertions it's worth google search */
/* on "Compile Time Assertions". It is part of the C11++ specification and */
/* all compilers that support the standard will have them (GCC, MSC inc)   */
/*-------------------------------------------------------------------------*/
#include <assert.h>								// Need for compile time static_assert

/* Check the code type structure size */
//static_assert(sizeof(CODE_TYPE) == 0x04, "Structure CODE_TYPE should be 0x04 bytes in size");
//static_assert(sizeof(CPU_ID) == 0x04, "Structure CPU_ID should be 0x04 bytes in size");
//static_assert(sizeof(struct GPIORegisters) == 0xA0, "Structure GPIORegisters should be 0xA0 bytes in size");
//static_assert(sizeof(struct SystemTimerRegisters) == 0x1C, "Structure SystemTimerRegisters should be 0x1C bytes in size");
//static_assert(sizeof(struct ArmTimerRegisters) == 0x1C, "Structure ArmTimerRegisters should be 0x1C bytes in size");
//static_assert(sizeof(struct IrqControlRegisters) == 0x28, "Structure IrqControlRegisters should be 0x28 bytes in size");
//static_assert(sizeof(struct MailBoxRegisters) == 0x40, "Structure MailBoxRegisters should be 0x40 bytes in size");

/***************************************************************************}
{     PUBLIC POINTERS TO ALL OUR RASPBERRY PI REGISTER BANK STRUCTURES	    }
****************************************************************************/
#define GPIO ((volatile __attribute__((aligned(4))) struct GPIORegisters*)(uintptr_t)(RPi_IO_Base_Addr + 0x200000))
#define SYSTEMTIMER ((volatile __attribute__((aligned(4))) struct SystemTimerRegisters*)(uintptr_t)(RPi_IO_Base_Addr + 0x3000))
#define IRQ ((volatile __attribute__((aligned(4))) struct IrqControlRegisters*)(uintptr_t)(RPi_IO_Base_Addr + 0xB200))
#define ARMTIMER ((volatile __attribute__((aligned(4))) struct  ArmTimerRegisters*)(uintptr_t)(RPi_IO_Base_Addr + 0xB400))
#define MAILBOX ((volatile __attribute__((aligned(4))) struct MailBoxRegisters*)(uintptr_t)(RPi_IO_Base_Addr + 0xB880))


/***************************************************************************}
{                       PUBLIC C INTERFACE ROUTINES                         }
{***************************************************************************/

/*==========================================================================}
{						   PUBLIC GPIO ROUTINES								}
{==========================================================================*/

/*-[gpio_setup]-------------------------------------------------------------}
. Given a valid GPIO port number and mode sets GPIO to given mode
. RETURN: true for success, false for any failure
. 30Jun17 LdB
.--------------------------------------------------------------------------*/
bool gpio_setup (uint_fast8_t gpio, GPIOMODE mode) {
	if (gpio > 54) return false;									// Check GPIO pin number valid, return false if invalid
	if (mode < 0 || mode > GPIO_ALTFUNC3) return false;				// Check requested mode is valid, return false if invalid
	uint_fast32_t bit = ((gpio % 10) * 3);							// Create bit mask
	uint32_t mem = GPIO->GPFSEL[gpio / 10];							// Read register
	mem &= ~(7 << bit);												// Clear GPIO mode bits for that port
	mem |= (mode << bit);											// Logical OR GPIO mode bits
	GPIO->GPFSEL[gpio / 10] = mem;									// Write value to register
	return true;													// Return true
}

/*-[gpio_output]------------------------------------------------------------}
. Given a valid GPIO port number the output is set high(true) or Low (false)
. RETURN: true for success, false for any failure
. 30Jun17 LdB
.--------------------------------------------------------------------------*/
bool gpio_output (uint_fast8_t gpio, bool on) {
	if (gpio > 54) return false;									// Check GPIO pin number valid, return false if invalid
	uint_fast32_t bit = 1 << (gpio % 32);							// Create mask bit
	if (on) {														// ON request
		GPIO->GPSET[gpio / 32] = bit;								// Set bit to make GPIO high output
	} else {
		GPIO->GPCLR[gpio / 32] = bit;								// Set bit to make GPIO low output
	}
	return true;													// Return true
}

/*-[gpio_input]-------------------------------------------------------------}
. Reads the actual level of the GPIO port number
. RETURN: true = GPIO input high, false = GPIO input low
. 30Jun17 LdB
.--------------------------------------------------------------------------*/
bool gpio_input (uint_fast8_t gpio) {
	if (gpio > 54) return false;									// Check GPIO pin number valid, return false if invalid
	uint_fast32_t bit = 1 << (gpio % 32);							// Create mask bit
	uint32_t mem = GPIO->GPLEV[gpio / 32];							// Read port level
	if (mem & bit) return true;										// Return true if bit set
	return false;													// Return false
}

/*-[gpio_checkEvent]-------------------------------------------------------}
. Checks the given GPIO port number for an event/irq flag.
. RETURN: true for event occured, false for no event
. 30Jun17 LdB
.-------------------------------------------------------------------------*/
bool gpio_checkEvent (uint_fast8_t gpio) {
	if (gpio > 54) return false;									// Check GPIO pin number valid, return false if invalid
	uint_fast32_t bit = 1 << (gpio % 32);							// Create mask bit
	uint32_t mem = GPIO->GPEDS[gpio / 32];							// Read event detect status register
	if (mem & bit) return true;										// Return true if bit set
	return false;													// Return false
}

/*-[gpio_clearEvent]-------------------------------------------------------}
. Clears the given GPIO port number event/irq flag.
. RETURN: true for success, false for any failure
. 30Jun17 LdB
.-------------------------------------------------------------------------*/
bool gpio_clearEvent (uint_fast8_t gpio) {
	if (gpio > 54) return false;									// Check GPIO pin number valid, return false if invalid
	uint_fast32_t bit = 1 << (gpio % 32);							// Create mask bit
	GPIO->GPEDS[gpio / 32] = bit;									// Clear the event from GPIO register
	return true;													// Return true
}

/*-[gpio_edgeDetect]-------------------------------------------------------}
. Sets GPIO port number edge detection to lifting/falling in Async/Sync mode
. RETURN: true for success, false for any failure
. 30Jun17 LdB
.-------------------------------------------------------------------------*/
bool gpio_edgeDetect (uint_fast8_t gpio, bool lifting, bool Async) {
	if (gpio > 54) return false;									// Check GPIO pin number valid, return false if invalid
	uint_fast32_t bit = 1 << (gpio % 32);							// Create mask bit
	if (lifting) {													// Lifting edge detect
		if (Async) GPIO->GPAREN[gpio / 32] = bit;					// Asynchronous lifting edge detect register bit set
			else GPIO->GPREN[gpio / 32] = bit;						// Synchronous lifting edge detect register bit set
	} else {														// Falling edge detect
		if (Async) GPIO->GPAFEN[gpio / 32] = bit;					// Asynchronous falling edge detect register bit set
			else GPIO->GPFEN[gpio / 32] = bit;						// Synchronous falling edge detect register bit set
	}
	return true;													// Return true
}

/*-[gpio_fixResistor]------------------------------------------------------}
. Set the GPIO port number with fix resistors to pull up/pull down.
. RETURN: true for success, false for any failure
. 30Jun17 LdB
.-------------------------------------------------------------------------*/
bool gpio_fixResistor (uint_fast8_t gpio, GPIO_FIX_RESISTOR resistor) {
	uint64_t endTime;
	if (gpio > 54) return false;									// Check GPIO pin number valid, return false if invalid
	if (resistor < 0 || resistor > PULLDOWN) return false;			// Check requested resistor is valid, return false if invalid
	GPIO->GPPUD = resistor;											// Set fixed resistor request to PUD register
	endTime = timer_getTickCount() + 2;								// We want a 2 usec delay
	while (timer_getTickCount() < endTime) {}						// Wait for the timeout
	uint_fast32_t bit = 1 << (gpio % 32);							// Create mask bit
	GPIO->GPPUDCLK[gpio / 32] = bit;								// Set the PUD clock bit register
	endTime = timer_getTickCount() + 2;								// We want a 2 usec delay
	while (timer_getTickCount() < endTime) {}						// Wait for the timeout
	GPIO->GPPUD = 0;												// Clear GPIO resister setting
	GPIO->GPPUDCLK[gpio / 32] = 0;									// Clear PUDCLK from GPIO
	return true;													// Return true
}

/*==========================================================================}
{						   PUBLIC TIMER ROUTINES							}
{==========================================================================*/

/*-[timer_getTickCount]-----------------------------------------------------}
. Get 1Mhz ARM system timer tick count in full 64 bit.
. The timer read is as per the Broadcom specification of two 32bit reads
. RETURN: tickcount value as an unsigned 64bit value
. 30Jun17 LdB
.--------------------------------------------------------------------------*/
uint64_t timer_getTickCount (void) {
	uint64_t resVal;
	uint32_t lowCount;
	do {
		resVal = SYSTEMTIMER->TimerHi; 								// Read Arm system timer high count
		lowCount = SYSTEMTIMER->TimerLo;							// Read Arm system timer low count
	} while (resVal != (uint64_t)SYSTEMTIMER->TimerHi);				// Check hi counter hasn't rolled in that time
	resVal = (uint64_t)resVal << 32 | lowCount;						// Join the 32 bit values to a full 64 bit
	return(resVal);													// Return the uint64_t timer tick count
}

/*-[timer_Wait]-------------------------------------------------------------}
. This will simply wait the requested number of microseconds before return.
. 02Jul17 LdB
.--------------------------------------------------------------------------*/
void timer_wait (uint64_t us) {
	us += timer_getTickCount();										// Add current tickcount onto delay
	while (timer_getTickCount() < us) {};							// Loop on timeout function until timeout
}


/*-[tick_Difference]--------------------------------------------------------}
. Given two timer tick results it returns the time difference between them.
. 02Jul17 LdB
.--------------------------------------------------------------------------*/
uint64_t tick_difference (uint64_t us1, uint64_t us2) {
	if (us1 > us2) {												// If timer one is greater than two then timer rolled
		uint64_t td = UINT64_MAX - us1 + 1;							// Counts left to roll value
		return us2 + td;											// Add that to new count
	}
	return us2 - us1;												// Return difference between values
}

/*==========================================================================}
{					     PUBLIC PI MAILBOX ROUTINES							}
{==========================================================================*/
#define MAIL_EMPTY	0x40000000		/* Mailbox Status Register: Mailbox Empty */
#define MAIL_FULL	0x80000000		/* Mailbox Status Register: Mailbox Full  */

/*-[mailbox_write]----------------------------------------------------------}
. This will execute the sending of the given data block message thru the
. mailbox system on the given channel.
. RETURN: True for success, False for failure.
. 04Jul17 LdB
.--------------------------------------------------------------------------*/
bool mailbox_write (MAILBOX_CHANNEL channel, uint32_t message) {
	uint32_t value;													// Temporary read value
	if (channel > MB_CHANNEL_GPU)  return false;					// Channel error
	message &= ~(0xF);												// Make sure 4 low channel bits are clear
	message |= channel;												// OR the channel bits to the value
	do {
		value = MAILBOX->Status1;									// Read mailbox1 status from GPU
	} while ((value & MAIL_FULL) != 0);								// Make sure arm mailbox is not full
	MAILBOX->Write1 = message;										// Write value to mailbox
	return true;													// Write success
}

/*-[mailbox_read]-----------------------------------------------------------}
. This will read any pending data on the mailbox system on the given channel.
. RETURN: The read value for success, 0xFEEDDEAD for failure.
. 04Jul17 LdB
.--------------------------------------------------------------------------*/
uint32_t mailbox_read (MAILBOX_CHANNEL channel) {
	uint32_t value;													// Temporary read value
	if (channel > MB_CHANNEL_GPU)  return 0xFEEDDEAD;				// Channel error
	do {
		do {
			value = MAILBOX->Status0;								// Read mailbox0 status
		} while ((value & MAIL_EMPTY) != 0);						// Wait for data in mailbox
		value = MAILBOX->Read0;										// Read the mailbox
	} while ((value & 0xF) != channel);								// We have response back
	value &= ~(0xF);												// Lower 4 low channel bits are not part of message
	return value;													// Return the value
}

/*-[mailbox_tag_message]----------------------------------------------------}
. This will post and execute the given variadic data onto the tags channel
. on the mailbox system. You must provide the correct number of response
. uint32_t variables and a pointer to the response buffer. You nominate the
. number of data uint32_t for the call and fill the variadic data in. If you
. do not want the response data back the use NULL for response_buf pointer.
. RETURN: True for success and the response data will be set with data
.         False for failure and the response buffer is untouched.
. 04Jul17 LdB
.--------------------------------------------------------------------------*/
bool mailbox_tag_message (uint32_t* response_buf,					// Pointer to response buffer
						  uint8_t data_count,						// Number of uint32_t data following
						  ...)										// Variadic uint32_t values for call
{
	uint32_t __attribute__((aligned(16))) message[32];
	va_list list;
	va_start(list, data_count);										// Start variadic argument
	message[0] = (data_count + 3) * 4;								// Size of message needed
	message[data_count + 2] = 0;									// Set end pointer to zero
	message[1] = 0;													// Zero response message
	for (int i = 0; i < data_count; i++) {
		message[2 + i] = va_arg(list, uint32_t);					// Fetch next variadic
	}
	va_end(list);													// variadic cleanup
	mailbox_write(MB_CHANNEL_TAGS, ARMaddrToGPUaddr(&message[0]));	// Write message to mailbox
	mailbox_read(MB_CHANNEL_TAGS);									// Wait for write response
	if (message[1] == 0x80000000) {
		if (response_buf) {											// If buffer NULL used then don't want response
			for (int i = 0; i < data_count; i++)
				response_buf[i] = message[2 + i];					// Transfer out each response message
		}
		return true;												// message success
	}
	return false;													// Message failed
}

/*==========================================================================}
{				     PUBLIC PI ACTIVITY LED ROUTINES						}
{==========================================================================*/
static bool ModelCheckHasRun = false;								// Flag set if model check has run
static uint_fast8_t ActivityGPIOPort = 47;							// Default GPIO for activity led is 47

/*-[set_Activity_LED]-------------------------------------------------------}
. This will set the PI activity LED on or off as requested. The SmartStart
. stub provides the Pi board autodetection so the right GPIO port is used.
. RETURN: True the LED state was successfully change, false otherwise
. 04Jul17 LdB
.--------------------------------------------------------------------------*/
bool set_Activity_LED (bool on) {
	/*switch (RPi_CpuId.PartNumber) {
		case 0xb76: {												// arm1176jzf-s AKA Pi1
			if (!ModelCheckHasRun) {								// Just check board isn't early Pi1 on GPIO16
				uint32_t model[4];
				ModelCheckHasRun = true;							// Set we have run the model check
				if (mailbox_tag_message(&model[0], 4,
					MAILBOX_TAG_GET_BOARD_REVISION, 4, 0, 0)) {
					if ((model[3] >= 0x0002) && (model[3] <= 0x000F))
					{												// Models A, B return 0002 to 000F
						ActivityGPIOPort = 16;						// GPIO port 16 is activity led
					} else ActivityGPIOPort = 47;					// GPIO port 47 activity as default
				}
			}
			gpio_output(ActivityGPIOPort, on);						// GPIO activity (port 16 or 47 depends on model) on/off
			return true;											// Return true
		}
		case 0xc07: {												// cortex-a7 AKA Pi2
			gpio_output(47, on);									// GPIO port 47 on/off
			return true;											// Return true
		}
		case 0xd03: {												// cortex-a53 AKA Pi3
			return (mailbox_tag_message(0, 5,
				MAILBOX_TAG_SET_GPIO_STATE,
				8, 8, 130, (uint32_t)on));							// Mailbox message,set GPIO port 130, on/off
		}
	}*/
	return false;													// Return false if above fail
}

/*==========================================================================}
{				     PUBLIC ARM CPU SPEED SET ROUTINES						}
{==========================================================================*/

/*-[ARM_setmaxspeed]--------------------------------------------------------}
. This will set the ARM cpu to the maximum. You can optionally print confirm
. message to screen but providing a print handler.
. RETURN: True maxium speed was successfully set, false otherwise
. 04Jul17 LdB
.--------------------------------------------------------------------------*/
bool ARM_setmaxspeed (printhandler prn_handler) {
	uint32_t Buffer[5];
	if (mailbox_tag_message(&Buffer[0], 5, MAILBOX_TAG_GET_MAX_CLOCK_RATE, 8, 8, 3, 0))
		if (mailbox_tag_message(&Buffer[0], 5, MAILBOX_TAG_SET_CLOCK_RATE, 8, 8, 3, Buffer[4])) {
			if (prn_handler) prn_handler("CPU frequency set to %u Hz\n", Buffer[4]);
			return true;											// Return success
		}
	return false;													// Max speed set failed
}
