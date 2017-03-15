/*
 * User.c
 *
 *  Created on: 14/03/2017
 *      Author: Alejandra Meza
 */

#include "User.h"

QueueHandle_t uartQueueHandle;

void createQueues(void){
	uartQueueHandle = xQueueCreate(10,sizeof(uint8_t *));
}

void uartSending_task(void *parameter){

	uint8_t* dataSend;

	while(1)
	{
		dataSend =uart_DataEcho();
		xQueueSend(uartQueueHandle,&dataSend,portMAX_DELAY);
	}

}

void uartReceiving_task(void *parameter){
	uint8_t dataReceived;
	while(1)
	{
		if(xQueueReceive(uartQueueHandle,&dataReceived,portMAX_DELAY)){
			PRINTF("%d\r\n",dataReceived);
		}
	}
}





