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



void uartSending_task(void *parameter);
void uartReceiving_task(void *parameter);
void createQueues(void);



#endif /* SOURCE_USER_H_ */
