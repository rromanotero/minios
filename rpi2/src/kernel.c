
/*
*
*	The Kernel
*
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "kernel.h"
#include "hal/hal.h"
#include "string.h"

#include "drivers/stdio/emb-stdio.h"			// Needed for printf
#include "drivers/sdcard/SDCard.h"

void kernel_init(void);
void io_init(void);
void show_init_info(uint8_t*);

char buffer[500];

void DisplayDirectory(const char*);

/*
 *		Kernel's entry point
**/
void main(uint32_t r0, uint32_t r1, uint32_t atags){

  //Init
  kernel_init();
  io_init();

  sdInitCard (&printf, &printf, true);

  /* Display root directory */
  printf("Directory (/): \n");
  DisplayDirectory("\\*.*");

  printf("\n");
  printf("Opening Alice.txt \n");

  HANDLE fHandle = sdCreateFile("Alice.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if (fHandle != 0) {
    uint32_t bytesRead;

    if ((sdReadFile(fHandle, &buffer[0], 500, &bytesRead, 0) == true))  {
        buffer[bytesRead-1] = '\0';  ///insert null char
        printf("File Contents: %s", &buffer[0]);
    }
    else{
      printf("Failed to read" );
    }

    // Close the file
    sdCloseHandle(fHandle);

  }

  //Welcome Msg Video
  printf( "\n\r\n\rWELCOME TO MINIOS (PI ZERO)\n\r" );
  printf( "\n\r$ " );

  uint8_t c;

	while (1){
    c = hal_io_serial_getc(SerialA);

#ifdef SERIAL_PRESENT
    printf( "%c", c );
#endif
  }
}



void io_init(void){

#ifdef SERIAL_PRESENT
  hal_io_serial_init();
#endif
  //NOTE: Past this point we can use
  //      use Serial

  show_init_info( "Serial Initialized" );

  if ( hal_io_video_init() == HAL_SUCCESS )
    show_init_info( "Video Initialized" );
  else
    show_init_info( "VIDEO INIT FAILED [x]" );

}

void kernel_init(void){
  //HAL INIT
  hal_io_init();
}

void show_init_info( uint8_t* msg ){
#ifdef SERIAL_PRESENT
  hal_io_serial_puts( SerialA, msg );
  hal_io_serial_puts( SerialA, "\n\r" );
#endif
}

void DisplayDirectory(const char* dirName) {
	HANDLE fh;
	FIND_DATA find;
	char* month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	fh = sdFindFirstFile(dirName, &find);							// Find first file
	do {
		if (find.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			printf("%s <DIR>\n", find.cFileName);
		else printf("%c%c%c%c%c%c%c%c.%c%c%c Size: %9lu bytes, %2d/%s/%4d, LFN: %s\n",
			find.cAlternateFileName[0], find.cAlternateFileName[1],
			find.cAlternateFileName[2], find.cAlternateFileName[3],
			find.cAlternateFileName[4], find.cAlternateFileName[5],
			find.cAlternateFileName[6], find.cAlternateFileName[7],
			find.cAlternateFileName[8], find.cAlternateFileName[9],
			find.cAlternateFileName[10],
			(unsigned long)find.nFileSizeLow,
			find.CreateDT.tm_mday, month[find.CreateDT.tm_mon],
			find.CreateDT.tm_year + 1900,
			find.cFileName);										// Display each entry
	} while (sdFindNextFile(fh, &find) != 0);						// Loop finding next file
	sdFindClose(fh);												// Close the serach handle
}
