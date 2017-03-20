/*
 * User.c
 *
 *  Created on: 14/03/2017
 *      Author: Alejandra Meza
 */

#include "User.h"


//QueueHandle_t uartQueueHandle;

uint8_t recv_bufferOne[] = {0};
uint8_t recv_bufferFour[]= {0};
uint8_t testBufferOne[] = {0};
uint8_t testBufferFour[] = {0};
size_t n;
uint8_t bufferCounter;


void chat_task(void *parameter)
{
	for(;;)
	{
		PRINTF("Hola opcion 8");
	}

}
