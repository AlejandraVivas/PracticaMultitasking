/*
 * User.c
 *
 *  Created on: 14/03/2017
 *      Author: Alejandra Meza
 */

#include "User.h"



extern uint8_t recv_buffer[1];

//QueueHandle_t uartQueueHandle;
//
//
////const char *to_send = "FreeRTOS UART driver example!\r\n";
//
//
//uint8_t readMemoryFlag = 0;
//
//void createQueues(void)
//{
//	uartQueueHandle = xQueueCreate(10,sizeof(uint8_t*));
//}
//
//
//void uartSending_task(void *parameter){
//	uint8_t dataSend[0];
//	for(;;)
//	{
//		//dataSend[0] = uart_DataEcho();
//		//PRINTF("TECLA PRESIONADA: %c\r\n", recv_buffer[0]);
//		xQueueSend(uartQueueHandle,&dataSend[0],0);
//	}
//
//}
//
//void uartReceiving_task(void *parameter){
//	uint8_t dataReceived[0];
//	for(;;)
//	{
//		if(xQueueReceive(uartQueueHandle,&dataReceived[0],portMAX_DELAY)){
//			//	PRINTF(" Valor recibido %c\r\n",dataReceived[0]);
//		}
//	}
//}

//void menuPrinting_task(void *parameter)
//{
//	uint8_t dataSend[0];
//	for(;;)
//	{
//		dataSend[0] = uart_DataEcho();
//		switch(recv_buffer[0])
//		{
//		case OPTION_1:
//			readMemoryFlag = 1;
//			break;
//		default:
//			break;
//		}
//	}
//}
//
//
//void readMemory_task(void *parameter)
//{
//	for(;;)
//	{
//		if(ESC != recv_buffer[0])
//		{
//			if(TRUE == readMemoryFlag)
//			{
//				readMemoryFlag = 0;
//				PRINTF("Opcion 1\r\n");
//			}
//		}
//		else if(ESC == recv_buffer[0])
//		{
//			PRINTF("salida");
//		}
//	}
//}

