
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
void input_output_init(void);
void sd_card_fs_demo();

/*
 *		Kernel's entry point
**/
void main(uint32_t r0, uint32_t r1, uint32_t atags){

  //Init
  kernel_init();
  input_output_init();

  //sd_card_fs_demo();   //<<-- Uncomment this to show File System/SD Card demo

  //Welcome Msg Video
  printf( "\n\r\n\rWELCOME TO MINIOS PI ZERO\n\r" );
  printf( "\n\r$ " );

  uint8_t c;

	while (1){
    c = hal_io_serial_getc(SerialA);
    printf( "%c", c );
  }
}

/*
* Initializes the kernel
*/
void kernel_init(void){

  hal_io_init();
  //console_init();
  //system_calls_init();
  //scheduler_init();
  //faults_init();

}

/*
* Initializes All IO
*/
void input_output_init(void){
  uint32_t video_init_res = HAL_FAILED;

#ifdef VIDEO_PRESENT
  video_init_res = hal_io_video_init();
#endif

#ifdef SERIAL_PRESENT
  hal_io_serial_init();
#endif
//NOTE: PAST THIS POINT WE CAN USE printf
//      (printf needs both serial and video inits to be called first)

  if ( video_init_res == HAL_SUCCESS )
    printf( "VIDEO INITIALIZED\n\r" );
  else
    printf( "VIDEO INIT FAILED [x]\n\r" );

    printf( "SERIAL INITIALIZED\n\r" );
}



/////////////////////////////////////////////////////////////////
////////////////    D E M O    C O D E    ///////////////////////
/////////////////////////////////////////////////////////////////

char buffer[500];
void DisplayDirectory(const char*);


void sd_card_fs_demo(){
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
