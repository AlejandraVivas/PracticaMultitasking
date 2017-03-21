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
#include "SerialMenu.h"

#define ENTER 0xD

#define TERMINALONE 1
#define TERMINALTWO 2


void createQueues(void);



#endif /* SOURCE_USER_H_ */
