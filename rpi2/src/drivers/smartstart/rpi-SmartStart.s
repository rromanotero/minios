/*
*   I got this code from Leon de Boer (see below), but there was no
*   copyright for this file, so I copy-pasted one from a different file
*
*   Rafael Roman Otero
*/

/*
{***************[ THIS CODE IS FREEWARE UNDER CC Attribution]***************}
{																                                            }
{     This sourcecode is released for the purpose to promote programming    }
{  on the Raspberry Pi. You may redistribute it and/or modify with the      }
{  following disclaimer and condition.                                      }
{																                                             }
{      The SOURCE CODE is distributed "AS IS" WITHOUT WARRANTIES AS TO      }
{   PERFORMANCE OF MERCHANTABILITY WHETHER EXPRESSED OR IMPLIED.            }
{   Redistributions of source code must retain the copyright notices to     }
{   maintain the author credit (attribution) .						              		}
{																			                                      }
*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++}
{			MEMORY HELPER ROUTINES PROVIDE BY RPi-SmartStart API		            }
{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* "PROVIDE C FUNCTION: uint32_t ARMaddrToGPUaddr (void* ARMaddress);" */
.section .text, "ax", %progbits
.balign	4
.globl ARMaddrToGPUaddr;
.type ARMaddrToGPUaddr, %function
.syntax unified
.arm
;@"================================================================"
;@ ARMaddrToGPUaddr -- Composite Pi1, Pi2 & Pi3 code
;@ C Function: uint32_t ARMaddrToGPUaddr (void* ARMaddress);
;@ Entry: R0 will have ARMAddress value
;@"================================================================"
ARMaddrToGPUaddr:
	ldr r1, =RPi_BusAlias
    ldr r1,[r1]								;@ Fetch bus alias
	orr r0, r0, r1							;@ Create bus address
	bx   lr									;@ Return
.balign	4
.ltorg										;@ Tell assembler ltorg data for this code can go here
.size	ARMaddrToGPUaddr, .-ARMaddrToGPUaddr


/* "PROVIDE C FUNCTION: uint32_t GPUaddrToARMaddr (uint32_t BUSaddress);" */
.section .text, "ax", %progbits
.balign	4
.globl GPUaddrToARMaddr;
.type GPUaddrToARMaddr, %function
.syntax unified
.arm
;@"================================================================"
;@ GPUaddrToARMaddr -- Composite Pi1, Pi2 & Pi3 code
;@ C Function: uint32_t GPUaddrToARMaddr (uint32_t BUSaddress);
;@ Entry: R0 will have GPUAddress value
;@"================================================================"
GPUaddrToARMaddr:
	ldr r1, =RPi_BusAlias
    ldr r1,[r1]								;@ Fetch bus alias
	bic r0, r0, r1							;@ Create arm address
	bx   lr									;@ Return
.balign	4
.ltorg										;@ Tell assembler ltorg data for this code can go here
.size	GPUaddrToARMaddr, .-GPUaddrToARMaddr

;@"*************************************************************************"
;@"          INTERNAL DATA FOR SMARTSTART NOT EXPOSED TO INTERFACE			"
;@"*************************************************************************"
.section ".data", "aw"
.balign 4

RPi_BusAlias	: .4byte 0;				// Address offset between VC4 physical address and ARM address needed for all DMA
