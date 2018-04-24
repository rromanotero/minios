/**
 * @file	hal_nvmem.c
 * @author  
 * @version 
 *
 * @brief Non-volatile memory part of the Hardware Abstraction Layer. 
 *
 */
#include <asf.h>
#include "hal.h"
#include "string.h"

static bool sdcard_init(uint32_t);
static bool fs_mount(void);

static FATFS fs;
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
	switch(mem_id){
		case NVMemSDCardFAT:	
			return sd_mmc_get_capacity(0) / (1024.0*1024.0); 
		case NVMemSDCard:		return 0;	//not implemented	
		case NVMemFlash:		return 0;	//not implemented	
		default:			return 0;	//error!
	}
}

/**
*	Non-volatile Memory FAT File Size
*
*	Returns the size in bytes of a FAT file. Only available when MemSDCardFAT 
*	has been initialized.	
*
*	@param fname file name 
*
*	@return size of the specified file 
*/
FILINFO fno; //Careful: fno can't be automatic
uint32_t hal_nvmem_fat_file_size( uint8_t* fname ){
	
	if( !sdcard_fat_init ) return 0;
		
	return f_stat( fname, &fno ) == FR_OK ? fno.fsize : 0;
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
	FIL fil;
	FRESULT fr;
	uint8_t* p_buffer = buffer;
	
	if( !sdcard_fat_init ) return 0;
	
	//Open the file
	if( f_open(&fil, fname, FA_OPEN_EXISTING | FA_READ) != FR_OK ){
		return 0; //0 bytes read
	}
	
	// Read all lines and return them
	uint32_t n = 0;
	uint32_t bytes_read;
	uint8_t s[2];
	
	//read until buffer is filled
	while ( n < buf_size ) {
		f_read(&fil, s, 1, &bytes_read);
		
		//EOF or error?
		if ( bytes_read != 1 )
			break;
		
		//put in buffer
		*p_buffer++ = s[0];
		
		n++;
	}
	
	f_close(&fil);
	
	return n;
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
	FIL fil;
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
	
	return bytes_written == buf_size;
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
	FILINFO fno;
	DIR dir;
	uint8_t *pc_fn;
	uint32_t num_files = 0; //files in the directory
	
	if( !sdcard_fat_init ) return 0;
	
#if _USE_LFN
	uint8_t c_lfn[_MAX_LFN + 1];
	fno.lfname = c_lfn;
	fno.lfsize = sizeof(c_lfn);
#endif
	
	if ( f_opendir( &dir, "0:" ) == FR_OK ){
		
		//read files in the directory (max_num_files at most)
		while( num_files < max_num_files && f_readdir(&dir, &fno) == FR_OK && fno.fname[0] != 0 ){
#if _USE_LFN
			pc_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			pc_fn = fno.fname;
#endif
			//Ignore directories
			if (fno.fattrib & AM_DIR){
				continue;
			}
			
			//populates list
			strcpy( list[num_files++], strlen(pc_fn) < MEM_FAT_MAX_FNAME_LENGTH ? pc_fn : "file name too large" );
			
		}//end while
	}
	
	return num_files;
}

static bool fs_mount(void){
	memset(&fs, 0, sizeof(FATFS));
	return f_mount(LUN_ID_SD_MMC_0_MEM, &fs) == FR_OK;
}

static bool sdcard_init( uint32_t num_attempts ){
	//check for SD Card to be inserted
	if( !gpio_pin_is_low(SD_MMC_0_CD_GPIO) ){
		return false;
	}
	
	//Init SD/MMC Stack
	sd_mmc_init();
	
	//Initializes Card
	while (	sd_mmc_check(0) != SD_MMC_OK ){
		if( num_attempts-- <= 0 ) return false;
		delay_ms(200);
	}
	
	return true;
}
