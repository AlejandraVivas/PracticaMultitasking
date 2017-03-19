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
size_t n;
uint8_t bufferCounter;


void chat_task(void *parameter)
{
	for(;;)
	{

			UART_RTOS_Receive(blueHandler(), recv_bufferFour, sizeof(recv_bufferFour), &n);
			UART_RTOS_Send(blueHandler(), recv_bufferFour, sizeof(recv_bufferFour));
			UART_RTOS_Send(teraHandler(), recv_bufferFour, sizeof(recv_bufferFour));
			UART_RTOS_Receive(teraHandler(), recv_bufferOne, sizeof(recv_bufferOne), &n);
			UART_RTOS_Send(teraHandler(), recv_bufferOne, sizeof(recv_bufferOne));
			UART_RTOS_Send(blueHandler(), recv_bufferOne, sizeof(recv_bufferOne));
	}
}

