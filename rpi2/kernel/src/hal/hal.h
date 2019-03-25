/*
*  HAL Header File
*
*	 Rafael Roman Otero
*/


#ifndef HAL_H
#define HAL_H

#include <stdbool.h>
#include <stdint.h>

#define HAL_SUCCESS  0
#define HAL_FAILED  1



//
// I R Q s
//
void hal_cpu_irqs_start( void(*)(void) );


//
// S V C
//
void hal_cpu_svc_start( void(*)(void) );


///
///  M E M  -  R E G I O N S
///
typedef uint32_t tMemRegionId;
enum tMemRegionId	{ MemRegSystem = 0, MemRegApp, MemRegSystemStack, MemRegUserStack };

typedef struct{
	tMemRegionId	id;
	uint8_t*		base;		/**< a pointer to the beginning of the region */
	uint32_t		size;		/**< size in bytes */
}tMemRegion;

void hal_memreg_init( void );
void hal_memreg_read( tMemRegionId, tMemRegion* );

///
///  N O N - V O L A T I L E   M E M O R Y
///

typedef uint32_t tNVMemId;
enum tNVMemId		{ NVMemSDCard = 0, NVMemSDCardFAT, NVMemFlash };

#define MEM_FAT_MAX_FNAME_LENGTH	30	// max file name length for the FAT file system (includes .ext where ext is extension)

void hal_nvmem_init( void );
bool hal_nvmem_start( tNVMemId );
double hal_nvmem_capacity( tNVMemId );
uint32_t hal_nvmem_fat_file_size( uint8_t* );
uint32_t hal_nvmem_fat_file_read( uint8_t*, uint8_t* , uint32_t );
bool hal_nvmem_fat_file_write( uint8_t*, uint8_t*, uint32_t );
uint32_t hal_nvmem_fat_file_list( uint8_t list[][MEM_FAT_MAX_FNAME_LENGTH+1], uint32_t );




///
///  V I D E O
///
typedef uint32_t VideoColor;

typedef struct{
  int32_t x;
  int32_t y;
}VideoXY;

typedef struct{
  VideoXY init;
  VideoXY end;
}VideoLine;


#define VIDEO_MAX_X 640      //Pixels per line
#define VIDEO_MAX_Y 480      //Num of lines

#define VIDEO_CHARACTER_WIDTH              4   //Unit Character dimensions
#define VIDEO_CHARACTER_HEIGHT             6   //(It's unit, because they all get
#define VIDEO_CHARACTER_HORIZONTAL_SPACE   3   // scaled by "size" when printing larger characters )
#define VIDEO_CHARACTER_VERTICAL_SPACE     4
#define VIDEO_CHARACTER_SPACE_BEFORE_END   2*VIDEO_CHARACTER_HEIGHT + VIDEO_CHARACTER_VERTICAL_SPACE

#define VIDEO_COLOR_WHITE   0xFFFF
#define VIDEO_COLOR_GREEN 	0x07E0
#define VIDEO_COLOR_BLUE    0x001F
#define VIDEO_COLOR_RED     0xF800
#define VIDEO_COLOR_BLACK   0x0000

void hal_io_init(void);
uint32_t hal_io_video_init( void );
void hal_io_video_put_pixel( VideoXY*, VideoColor );
void hal_io_clear_screen( void );
void hal_io_video_line( VideoXY*, VideoXY*, VideoColor );
void hal_io_video_putc_x_y( uint32_t x, uint32_t y, uint8_t c, uint32_t size, VideoColor color );
void hal_io_video_putc( uint8_t c, uint32_t size, VideoColor color  );
void hal_io_video_puts( uint8_t*, uint32_t, VideoColor );


///
///  S E R I A L
///
typedef uint32_t SerialId;

enum SerialId { SerialA = 0, SerialB };

void hal_io_serial_init( void );
void hal_io_serial_putc( SerialId, uint8_t );
uint8_t hal_io_serial_getc( SerialId );
void hal_io_serial_puts( SerialId, uint8_t* );



#endif
