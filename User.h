/*
 * User.h
 *
 *  Created on: 14/03/2017
 *      Author: Alejandra Meza
 */

#ifndef SOURCE_USER_H_
#define SOURCE_USER_H_

#include "UART.h"
#include "queue.h"
#include "stdio.h"
#include "fsl_debug_console.h"
#include "DataTypeDefinitions.h"

#define OPTION_1 0x31
#define OPTION_2 0x32
#define OPTION_3 0x33
#define ESC 0x1B

void uartSending_task(void *parameter);
void uartReceiving_task(void *parameter);
void readMemory_task(void *parameter);
void createQueues(void);




#endif /* SOURCE_USER_H_ */
