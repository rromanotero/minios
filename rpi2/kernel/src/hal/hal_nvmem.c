/**
 * @file	hal_nvmem.c
 * @author
 * @version
 *
 * @brief Non-volatile memory part of the Hardware Abstraction Layer.
 *
 */
#include <stdbool.h>
#include <stdint.h>
#include "../drivers/sdcard/SDCard.h"
#include "../drivers/stdio/emb-stdio.h"
#include "string.h"
#include "hal.h"

static bool sdcard_init(uint32_t);
static bool fs_mount(void);

//static FATFS fs;
static bool sdcard_fat_init = false;

/**
*	HAL Non-Volatile Memory Init
*
*	Initializes Non-volatile memory. This function must be called after
*	HAL CPU Init. That is: hal_cpu_init(); hal_nvmem_init();...
*
*/
void hal_nvmem_init(void){
	//nothing to initialize...
	//for consistency...
	//and compatibility if future versions require initialization
}

/**
*	Non-volatile Memory start
*
*	Initializes the specified non-volatile memory
*
*	@param mem_id id of the memory to be initialized
*
*	@return true if the memory was initialized correctly, false otherwise.
*/
bool hal_nvmem_start( tNVMemId mem_id ){

	switch(mem_id){
		case NVMemSDCardFAT:				//SD Card + FAT
			sdcard_fat_init = true;
			return sdcard_init(10) && fs_mount();
		case NVMemSDCard:		return false;							//unimplemented (raw access to sd card)
		case NVMemFlash:		return false;							//unimplemented (on board flash)
		default:			return false;							//error!
	}
}

/**
*	Non-volatile Memory Capacity
*
*	Returns the capacity in GB of the specified memory
*
*	@param mem_id id of the memory
*
*	@return memory capacity
*/
double hal_nvmem_capacity( tNVMemId mem_id ){
	/*
	switch(mem_id){
		case NVMemSDCardFAT:
			return sd_mmc_get_capacity(0) / (1024.0*1024.0);
		case NVMemSDCard:		return 0;	//not implemented
		case NVMemFlash:		return 0;	//not implemented
		default:			return 0;	//error!
	}*/
}

/**
*	Non-volatile Memory FAT File Size
*
*  N O T    W O RK I N G    C O R R E C T L Y
*	    Currently it return not 0 if the file was found, and 0 if the file does not exist
*	    So it's good for telling if a file exists or not, but it always returns the same
*	    file size, for any given file.
*
*	Returns the size in bytes of a FAT file. Only available when MemSDCardFAT
*	has been initialized.
*
*	@param fname file name
*
*	@return size of the specified file
*/
uint32_t hal_nvmem_fat_file_size( uint8_t* fname ){

	//--------------------------------------------
	// N O T   W O R K I N G, see comments above.
	//--------------------------------------------

	if( !sdcard_fat_init ) return 0;

	HANDLE fh;
	FIND_DATA find;
	uint32_t size = 0;

	// Find all files in root (we don't support directories)
	fh = sdFindFirstFile("\\*.*", &find);

	//find file
	bool found = false;

	do {
		if( strcmp( fname, find.cFileName  ) == 0 ){
			size = (unsigned long)find.nFileSizeLow;
			break;
		}
	} while ( sdFindNextFile(fh, &find) != 0 );

	sdFindClose(fh);

	return size;
}

/**
*	NOn-volatile Memory FAT File read
*
*	Reads the specified file and copy its content to the specified buffer.
*	It copies at most buf_size bytes.
*	Note: It reads raw bytes. Not a null-terminated string.
*
*	@param fname file name
*	@param buffer buffer
*	@param buf_size	size of the buffer in bytes
*
*	@return the total number of bytes copied to the buffer
*/
uint32_t hal_nvmem_fat_file_read( uint8_t* fname, uint8_t* buffer, uint32_t buf_size ){

	HANDLE f_handle = sdCreateFile( fname, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if( f_handle == 0 )
		return 0; 	//file not found (I'm guessing!)

	uint32_t bytes_read;

	if ((sdReadFile(f_handle, buffer, buf_size, &bytes_read, 0) == true)) {
		//Read file fine
		return bytes_read;
	}
	else{
		//failed to read
		return 0;
	}

	// Close the file
	sdCloseHandle(f_handle);

}

/**
*	Non-volatile Memory FAT File write
*
*	Writes buf_size bytes from the specified buffer to the specified
*	file.  If the file does not exist it creates it. If the file already
*	exists, it replaces it.
*
*	@param fname file name
*	@param buffer buffer
*	@param buf_size	size of the buffer in bytes
*
*	@return true is writing was successful
*/
bool hal_nvmem_fat_file_write( uint8_t* fname, uint8_t* buffer, uint32_t buf_size ){
	/*FIL fil;
	uint32_t bytes_written;

	if( !sdcard_fat_init ) return false;

	//  --- Create an empty new file  ---
	if( f_open(&fil, fname, FA_OPEN_EXISTING ) == FR_OK ){
		// --- File exist ---

		//Close file and Delete
		f_close(&fil);						//Don't forget to close
		if( f_unlink( fname ) != FR_OK )
		return false; //deleting file failed

		//Create and open file
		if ( f_open(&fil, fname, FA_CREATE_ALWAYS | FA_WRITE ) != FR_OK ){
			return false; // creating file failed
		}
	}
	else{
		// --- File does not exist ---

		//Create and open file
		if ( f_open(&fil, fname, FA_CREATE_ALWAYS | FA_WRITE ) != FR_OK ){
			return false; // creating file failed
		}
	}

	//write
	f_write( &fil, buffer, buf_size, &bytes_written );

	//close
	f_close(&fil);

	return bytes_written == buf_size;*/
	return 0;
}

/**
*	Non-volatile Memory FAT File List
*
*	Obtains the list of files found in the root directory
*	of the SD card. It reads at most max_num_files files. The max
*	length of a file name is given by MEM_FAT_MAX_FNAME_LENGTH.
*	Example of how to read:
*
*	uint8_t list[10][MEM_FAT_MAX_FNAME_LENGTH+1];
*
*	if( ( n = hal_nvmem_fat_file_list( list, 10 ) ) > 0 ){
*		//at this point each list[i] holds a file name
*	}
*
*	@param list				buffers where names will be copied to.
*	@param max_num_files	maximum number of file names to copy to the list
*
*	@return number of files found
*/
uint32_t hal_nvmem_fat_file_list( uint8_t list[][MEM_FAT_MAX_FNAME_LENGTH+1], uint32_t max_num_files ){

	HANDLE fh;
	FIND_DATA find;

	fh = sdFindFirstFile("\\*.*", &find);							// Find all files in root

	uint32_t file_num = 0;
	do {
		//save each file name
		memcpy( list[file_num], find.cFileName, MEM_FAT_MAX_FNAME_LENGTH );
		list[file_num][MEM_FAT_MAX_FNAME_LENGTH] = '\0';		//In case file name was truncated

		file_num++;

	} while ( sdFindNextFile(fh, &find) != 0 && file_num <= max_num_files);

	sdFindClose(fh);												// Close the serach handle

	return file_num;

// We can actually work with directories, but I don't feel like doing That
//right now, so I leave this SAMPLE CODE for reference:


/*
Callled like this:

  DisplayDirectory("\\*.*");
*/
/*
Definition of DisplayDirectory:

HANDLE fh;
FIND_DATA find;
char* month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
fh = sdFindFirstFile(dirName, &find);							// Find first file
do {
	if (find.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		printf_serial("%s <DIR>\n", find.cFileName);
	else printf_serial("%c%c%c%c%c%c%c%c.%c%c%c Size: %9lu bytes, %2d/%s/%4d, LFN: %s\n",
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




*/

}

static bool fs_mount(void){
	/*
		Nothing to do in the PI. Left for compatibility purposes.
	*/
	return true;
}

uint8_t sd_card_init_info[200];

static bool sdcard_init( uint32_t num_attempts ){

	sdInitCard (&printf_null, &printf_serial, true); //errors are printed to serial

	return true;
}
