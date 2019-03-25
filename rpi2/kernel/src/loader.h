/**
 * @file	iostream.c
 * @author  
 * @version 
 *
 * @brief MiniOS IO Streams
 *	
 */


#ifndef LOADER_H_
#define LOADER_H_

#define LOADER_NO_APP_FOUND			0
#define LOADER_APP_TOO_LARGE		1
#define LOADER_LOAD_SUCCESS			2 //app loaded successfully (regardless of core returned by app)

/**
* A structure to represent Apps
*/

void loader_init(void);
uint32_t loader_load_app( uint8_t*, tMemRegion*, uint32_t* );

#endif /* LOADER_H_ */