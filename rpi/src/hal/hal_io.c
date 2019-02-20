/*
*  HAL IO
*
*	 Rafael Roman Otero
*
*/

#include <stdint.h>
#include "hal.h"
#include "fonts.h"
#include "../boot/rpi-smartstart.h"
#include "../drivers/stdio/emb-stdio.h"	

static void uart0_init(void);
static void uart0_putc(uint8_t);
static uint8_t uart0_getc(void);

static void draw_character_raw( uint32_t, uint32_t, VideoFont*, uint32_t, VideoColor );
static void setup_fonts(void);
static uint32_t x_y_to_raw(uint32_t, uint32_t);
static int32_t abs(int32_t);
static void str(uint32_t, uint32_t);
static uint32_t ldr(uint32_t);
static void delay(uint32_t);

extern uint32_t _hal_io_video_init(void);
extern void _hal_io_video_put_pixel_raw( uint32_t, VideoColor );

/*
*  HAL IO Init
*/

void hal_io_init( void ){
	fonts_init();
}

/*
*  HAL IO Video Init
*/
uint32_t hal_io_video_init( void ){
	//return _hal_io_video_init();   <<<---- This is defined in ARM assembly,
	//																			 any 32-bit assembly will break
	//																			 the build
	return 1;
}


/*
*  HAL IO Video Puts
*
*/
void hal_io_video_puts( uint8_t* string, uint32_t size, VideoColor color ){
		while(*string)
			hal_io_video_putc( *string++, size, color );
}

/*
*  HAL IO Video Putc
*
*/
#define X_ORIGIN 	VIDEO_CHARACTER_WIDTH
#define Y_ORIGIN  VIDEO_CHARACTER_HEIGHT*2
uint32_t curr_x=X_ORIGIN;
uint32_t curr_y=Y_ORIGIN;
void hal_io_video_putc( uint8_t c, uint32_t size, VideoColor color  ){

	if( c == '\n' ){
			curr_y = ( curr_y
				 				+ VIDEO_CHARACTER_HEIGHT*size
								+ VIDEO_CHARACTER_VERTICAL_SPACE ) %  (VIDEO_MAX_Y-VIDEO_CHARACTER_HEIGHT);
			return;
	}
	if( c == '\r' ){
			curr_x = X_ORIGIN;
			return;
	}

	hal_io_video_putc_x_y( curr_x, curr_y, c, size, color );

	//Move cursor
	curr_x = ( curr_x
						+ VIDEO_CHARACTER_WIDTH*size
						+ VIDEO_CHARACTER_HORIZONTAL_SPACE*size )  % (VIDEO_MAX_X-VIDEO_CHARACTER_WIDTH);
}

/*
*  HAL IO Video Putc XY
*
*/
void hal_io_video_putc_x_y( uint32_t x, uint32_t y, uint8_t c, uint32_t size, VideoColor color  ){

	if( !fonts_is_implemented(c) ){
		return;
	}

	//double character so it looks bold
	draw_character_raw(x, y, fonts_char_to_font(c), size, color );
	draw_character_raw(x+1, y+1, fonts_char_to_font(c), size, color );

}

static void draw_character_raw( uint32_t x, uint32_t y, VideoFont* c, uint32_t size, VideoColor color ){

	//Draw as per specs
	VideoXY init, end;

	for( int i=0; i<c->num_lines; i++ ){
			//For all lines

			//Set up (x0,y0)*size
			init.x 	= x + c->lines[i].init.x * size;
			init.y 	= y + c->lines[i].init.y * size;

			//Set up (x1,y1)*size
			end.x 	= x + c->lines[i].end.x * size;
			end.y 	= y + c->lines[i].end.y * size;

			hal_io_video_line( &init, &end, color );
	}

}


/*
*  HAL IO Line
*
*  It used Brasenham's Line Algorithm (Developed at IBM in 1962!).
*
* Actual code from here:
*       https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
*
*/
void hal_io_video_line( VideoXY* init, VideoXY* end, VideoColor color ){
	int32_t x0= init->x;
	int32_t y0= init->y;
	int32_t x1= end->x;
	int32_t y1= end->y;

	int32_t dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int32_t dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int32_t err = (dx>dy ? dx : -dy)/2, e2;

	VideoXY pos;

	for(;;){
		pos.x = x0;
		pos.y = y0;

		hal_io_video_put_pixel( &pos, color);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

static int32_t abs(int32_t v){
		if(v<0) return v*(-1);
		else return v;
}

/*
*  HAL IO Video PutPixel
*/
void hal_io_video_put_pixel( VideoXY* pos, VideoColor color ){
	//_hal_io_video_put_pixel_raw(  x_y_to_raw(pos->x,pos->y), color );   <<<---- This is defined in ARM assembly,
	//																			 any 32-bit assembly will break
	//																			 the build
	SmartStartPutPixelRaw( &printf, x_y_to_raw(pos->x,pos->y), color );
}

static uint32_t x_y_to_raw(uint32_t x, uint32_t y){
	//For some reason On a 640x480
	//x has 640*2-1 pixels, not 640-1 vo.0v
	#define FIXED_VIDEO_MAX_X		2*VIDEO_MAX_X

	return y*FIXED_VIDEO_MAX_X + 2*x;
}

/*
*  HAL IO Serial Init
*/
void hal_io_serial_init( void ){
	uart0_init();
}

/*
*  HAL IO Serial Puts
*/
void hal_io_serial_puts( SerialId serial_id, uint8_t* str){
	while(*str)
		hal_io_serial_putc( serial_id, *str++);
}

/*
*  HAL IO Serial Putc
*/
void hal_io_serial_putc( SerialId id, uint8_t c ){
	switch(id){
		case SerialA: uart0_putc(c); break;
		case SerialB: /* Unimplemented*/ break;
	}
}

/*
*  HAL IO Serial Getc
*/
uint8_t hal_io_serial_getc( SerialId id ){
	switch(id){
		case SerialA: return uart0_getc();
		case SerialB: /* Unimplemented*/ return 0;
	}
}


/*
*   ALL UART0 (putc, getc, init) FROM  HERE:
*	    https://wiki.osdev.org/Raspberry_Pi_Bare_Bones#Building_a_Cross-Compiler
*/

enum{
    GPIO_BASE = 0x3F200000,         // For raspi2 & 3, (0x20200000 for raspi1)
    GPPUD = (GPIO_BASE + 0x94),     // Controls actuation of pull up/down to ALL GPIO pins.
    GPPUDCLK0 = (GPIO_BASE + 0x98), // Controls actuation of pull up/down for specific GPIO pin.
    UART0_BASE = 0x3F201000,        // The base address for UART.
                                    // (for raspi2 & 3, 0x20201000 for raspi1 )
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};

static void uart0_putc(uint8_t c){
	//wait for it to be ready
	while ( ldr(UART0_FR) & (1 << 5) );

	//write
	str(UART0_DR, c);
}

static uint8_t uart0_getc(void){
	//wait for it to be ready
	while ( ldr(UART0_FR) & (1 << 4) );

	//write
	return ldr(UART0_DR);
}

static void uart0_init(void){
	// Disable UART0.
	str(UART0_CR, 0x00000000);

	// Setup the GPIO pin 14 && 15.
	//  --- is this missing? -- Rafael

	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	str(GPPUD, 0x00000000);
	delay(150);
	// Disable pull up/down for pin 14,15 & delay for 150 cycles.
	str(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
	// Write 0 to GPPUDCLK0 to make it take effect.
	str(GPPUDCLK0, 0x00000000);
	// Clear pending interrupts.
	str(UART0_ICR, 0x7FF);
	// Set integer & fractional part of baud rate.
	// Divider = UART_CLOCK/(16 * Baud)
	// Fraction part register = (Fractional part * 64) + 0.5
	// UART_CLOCK = 3000000; Baud = 115200.

	// Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
	str(UART0_IBRD, 1);
	// Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
	str(UART0_FBRD, 40);
	// Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
	str(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
	// Mask all interrupts.
	str(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
												 (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
	// Enable UART0, receive & transfer part of UART.
	str(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));

}

static void str(uint32_t address, uint32_t v){
	*(volatile uint32_t*)address = v;
}

static  uint32_t ldr(uint32_t address){
	return *(volatile uint32_t*)address;
}

static void delay(uint32_t count) {
	/*
	*	From
	*	https://wiki.osdev.org/Raspberry_Pi_Bare_Bones#Building_a_Cross-Compiler
	*/
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : "=r"(count): [count]"0"(count) : "cc");
}
