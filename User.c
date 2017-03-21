/*
 * User.c
 *
 *  Created on: 14/03/2017
 *      Author: Alejandra Meza
 */

#include "User.h"


//QueueHandle_t uartQueueHandle;

TaskHandle_t chatHandle = NULL;

uint8_t recv_bufferOne[] = {0};
uint8_t recv_bufferFour[]= {0};
uint8_t testBufferOne[] = {0};
uint8_t testBufferFour[] = {0};
size_t n;
uint8_t bufferCounter;

extern uint8_t uart0Data;
extern uint8_t uart3Data;

extern uint8_t TerminalOneChat;
extern uint8_t TerminalTwoChat;

uint8_t TerminalOneString[] = "Terminal 1:\r\n";
uint8_t TerminalTwoString[] = "Terminal 2:\r\n";
uint8_t NewLineString[] = "\r\n";

uint8_t bufferOneCounter = 0;
uint8_t bufferFourCounter = 0;

uint8_t clearingCommand[] = "\033[0;30;46m"; //clearing screen
uint8_t blueScreeCommand[] = "\033[2J"; //Blue background and white letters
uint8_t xandyPositioning[] = "\033[1;1H"; //x and y cursor position


void CreateChatTask(void)
{
	xTaskCreate(chat_task, "Chat_Task", configMINIMAL_STACK_SIZE, NULL,2 , &chatHandle);
}

void chat_task(void *pvParameters)
{
//		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
//		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
//		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
//		if(TERMINALONE == TerminalOneChat){
//			UART_WriteBlocking(UART0, TerminalOneString,sizeof(TerminalOneString));
//				while((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEMO_UART0)))
//				{
//					UART_WriteByte(DEMO_UART0, uart0Data);
//					if(ENTER != uart0Data)
//					{
//						testBufferOne[bufferOneCounter] = uart0Data;
//						bufferOneCounter++;
//					}
//					else if(ENTER == uart0Data)
//					{
//						UART_WriteBlocking(UART3, testBufferOne,sizeof(testBufferOne));
//						UART_WriteBlocking(UART3, NewLineString,sizeof(NewLineString));
//						UART_WriteBlocking(UART0, testBufferOne,sizeof(testBufferOne));
//						UART_WriteBlocking(UART0, NewLineString,sizeof(NewLineString));
//					}
//				}
//					while(1)
//					{
//						else if(TERMINALTWO == TerminalTwoChat)
//						{
//							uart3Data = UART_ReadByte(DEMO_UART3);
//							UART_WriteByte(DEMO_UART3, uart3Data);
//							if(ENTER != uart3Data)
//							{
//								testBufferFour[bufferFourCounter] = uart3Data;
//								bufferFourCounter++;
//							}
//							else if(ENTER == uart3Data)
//							{
//								UART_WriteBlocking(UART0, testBufferFour,sizeof(testBufferFour));
//								UART_WriteBlocking(UART0, testBufferFour,sizeof(testBufferFour));
//						}
//						}
//					}
//
//		}
uint8_t contador;

	for(;;)
	{
		if(ESC != uart0Data)
		{
			PRINTF("Numero de contador: %d\r\n", contador);
			contador++;
		}
		else if(ESC == uart0Data)
		{
			if(chatHandle != NULL)
				{
				vTaskDelete(NULL);
				}
			vTaskDelete(chatHandle);
		}
	}
		//vTaskSuspend( NULL );
}

