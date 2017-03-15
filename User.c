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

	uint8_t dataSend[1];

	while(1)
	{
		PRINTF(" Valor enviado %c\r\n",dataSend[0]);
		dataSend[0] =uart_DataEcho();
		xQueueSend(uartQueueHandle,&dataSend[0],0);
	}

}

void uartReceiving_task(void *parameter){
	uint8_t dataReceived[1];
	while(1)
	{
		if(xQueueReceive(uartQueueHandle,&dataReceived[0],portMAX_DELAY)){
			//PRINTF(" Valor recibido %c\r\n",dataReceived[0]);
		}
	}
}





