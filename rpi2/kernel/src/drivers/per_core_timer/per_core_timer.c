

/*
*   Per core timer:
*
*   Adapted from here:
*       https://github.com/eggman/raspberrypi/blob/master/qemu-raspi2/timer01/boot.S
*
*/


#include <stddef.h>
#include <stdint.h>

#define CORE0_TIMER_IRQCNTL 0x40000040
#define CORE0_IRQ_SOURCE 0x40000060

static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t*)reg = data;
}

// Memory-Mapped I/O input
static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t*)reg;
}


void per_core_timer_routing_core0cntv_to_core0irq(void)
{
    mmio_write(CORE0_TIMER_IRQCNTL, 0x08);
}

uint32_t per_core_timer_read_core0timer_pending(void)
{
    uint32_t tmp;
    tmp = mmio_read(CORE0_IRQ_SOURCE);
    return tmp;
}

void enable_cntv(void)
{
    uint32_t cntv_ctl;
    cntv_ctl = 1;
	asm volatile ("mcr p15, 0, %0, c14, c3, 1" :: "r"(cntv_ctl) ); // write CNTV_CTL
}

void disable_cntv(void)
{
    uint32_t cntv_ctl;
    cntv_ctl = 0;
	asm volatile ("mcr p15, 0, %0, c14, c3, 1" :: "r"(cntv_ctl) ); // write CNTV_CTL
}

uint64_t read_cntvct(void)
{
	uint64_t val;
	asm volatile("mrrc p15, 1, %Q0, %R0, c14" : "=r" (val));
	return (val);
}

uint64_t read_cntvoff(void)
{
	uint64_t val;
    asm volatile("mrrc p15, 4, %Q0, %R0, c14" : "=r" (val));
	return (val);
}

uint32_t read_cntv_tval(void)
{
    uint32_t val;
	asm volatile ("mrc p15, 0, %0, c14, c3, 0" : "=r"(val) );
    return val;
}

uint32_t per_core_timer_read_cntfrq(void)
{
    uint32_t val;
	   asm volatile ("mrc p15, 0, %0, c14, c0, 0" : "=r"(val) );
    return val;
}


void per_core_timer_write_cntv_tval(uint32_t val)
{
	asm volatile ("mcr p15, 0, %0, c14, c3, 0" :: "r"(val) );
    return;
}


void per_core_timer_enable(uint32_t cntv_val)
{
    //init
    per_core_timer_routing_core0cntv_to_core0irq();
    enable_cntv();
}
