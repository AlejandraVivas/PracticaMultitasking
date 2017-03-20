/*
 * Memory.h
 *
 *  Created on: Mar 20, 2017
 *      Author: peter
 */

#ifndef SOURCE_MEMORY_H_
#define SOURCE_MEMORY_H_


#include "FreeRTOS.h"
#include "task.h"
#include "fsl_port.h"

#define Memory_setup_writetask_PRIORITY (4)
#define Memory_setup_readtask_PRIORITY (3)

#define Memory_Slave_Address (0x50)

void Memory_readTask(void *pvParameters);
void Memory_writeTask(void *pvParameters);
static status_t  Memory_init(void);


#endif /* SOURCE_MEMORY_H_ */
