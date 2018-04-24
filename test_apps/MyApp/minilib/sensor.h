/*
 * sensor.h
 * 
 * Sensors
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint-gcc.h>	

typedef uint32_t tSensorId;			
enum tSensorId	{ SensorLight = 0, SensorTemp };	
	
void sensor_start_poll( tSensorId ); 
//void sensor_start_ev( tSensorId ); //not supported yet
uint32_t sensor_read( tSensorId );

#endif /* SENSOR_H_ */