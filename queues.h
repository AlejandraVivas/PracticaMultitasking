/*
 * queues.h
 *
 *  Created on: Mar 22, 2017
 *      Author: peter
 */

#ifndef SOURCE_QUEUES_H_
#define SOURCE_QUEUES_H_


typedef struct {
	uint8_t slaveAddress;
	uint8_t direction;
	uint8_t subaddress;
	uint8_t subaddressSize;
	uint8_t data;
}I2C_stuctData_t;


#endif /* SOURCE_QUEUES_H_ */
