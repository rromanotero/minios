
#ifndef HAL_H
#define HAL_H

#define HAL_SUCCESS  0
#define HAL_FAILED  1

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

#define VIDEO_CHARACTER_WIDTH             10   //Unit Character dimensions
#define VIDEO_CHARACTER_HEIGHT            16   //(It's unit, because they all get
#define VIDEO_CHARACTER_HORIZONTAL_SPACE   3   // scaled by "size" when printing larger characters )
#define VIDEO_CHARACTER_VERTICAL_SPACE     9

#define VIDEO_COLOR_WHITE   0xFFFF
#define VIDEO_COLOR_GREEN 	0x07E0
#define VIDEO_COLOR_BLUE    0x001F
#define VIDEO_COLOR_RED    0xF800

void hal_io_init(void);
uint32_t hal_io_video_init( void );
void hal_io_video_put_pixel( VideoXY*, VideoColor );
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
