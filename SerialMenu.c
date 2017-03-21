/*
 * SerialMenu.c
 *
 *  Created on: 19/03/2017
 *      Author: Alejandra Meza
 */

#include "SerialMenu.h"

TaskHandle_t chatHandle = NULL;
uint8_t clearingCommand[] = "\033[0;30;46m"; //clearing screen
uint8_t blueScreeCommand[] = "\033[2J"; //Blue background and white letters
uint8_t xandyPositioning[] = "\033[1;1H"; //x and y cursor position

uint8_t menuString[] = "Menu\r\n";
uint8_t optionString[] = "Opciones:\r\n";
uint8_t readMemoryString[] = "1) Leer Memoria\r\n";
uint8_t writeMemoryString[] = "2) Escribir Memoria\r\n";
uint8_t setHourString[] = "3) Establecer Hora\r\n";
uint8_t setDateString[] = "4) Establecer Fecha\r\n";
uint8_t hourFormatString[] = "5) Formato de Hora\r\n";
uint8_t readHourString[] = "6) Leer Hora\r\n";
uint8_t readDateString[] = "7) Leer Fecha\r\n";
uint8_t chatString[] = "8) Comunicacion con terminal 2\r\n";
uint8_t ecoString[] = "9) Eco en LCD\r\n";

uint8_t uart0Data;
uint8_t uart3Data;

uint8_t uartFlag;

uint8_t optionFlag;

uint8_t optionOneFlag = FALSE;
uint8_t optionTwoFlag = FALSE;
uint8_t optionThreeFlag = FALSE;
uint8_t optionFourFlag = FALSE;
uint8_t optionFiveFlag = FALSE;
uint8_t optionSixFlag = FALSE;
uint8_t optionSevenlag = FALSE;
uint8_t optionEightFlag = FALSE;
uint8_t optionNineFlag = FALSE;

uint8_t TerminalOneChat;
uint8_t TerminalTwoChat;



uart_handle_t g_uart0Handle;
uart_handle_t g_uart3Handle;

uint8_t demoRingBufferUart0[16];
volatile uint16_t txIndexUart0; /* Index of the data to send out. */
volatile uint16_t rxIndexUart0; /* Index of the memory to save new arrived data. */

uint8_t demoRingBufferUart3[16];
volatile uint16_t txIndexUart3; /* Index of the data to send out. */
volatile uint16_t rxIndexUart3; /* Index of the memory to save new arrived data. */

uart_transfer_t xfer;
volatile bool txOnGoing = false;

void DEMO_UART0_IRQHandler(void)
{

	/* If new data arrived. */
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(DEMO_UART0))
	{
		uart0Data = UART_ReadByte(DEMO_UART0);
		optionFlag = uart0Data;
		TerminalOneChat = UART0FLAG;
		if (((rxIndexUart0 + 1) % 16) != txIndexUart0)
		{
			demoRingBufferUart0[rxIndexUart0] = uart0Data;
			rxIndexUart0++;
			rxIndexUart0 %= 16;
		}
	}
}

void DEMO_UART3_IRQHandler(void)
{

	/* If new data arrived. */
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(DEMO_UART3))
	{
		uart3Data = UART_ReadByte(DEMO_UART3);
		optionFlag = UART_ReadByte(DEMO_UART3);
		TerminalTwoChat = UART3FLAG;
		if (((rxIndexUart3 + 1) % 16) != txIndexUart3)
		{
			demoRingBufferUart3[rxIndexUart3] = uart0Data;
			rxIndexUart3++;
			rxIndexUart3 %= 16;
		}
	}
}



void mainMenu_task(void *pvParameters)
{
	UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
	UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
	UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
	UART_WriteBlocking(DEMO_UART0, menuString, sizeof(menuString) / sizeof(menuString[0]));
	UART_WriteBlocking(DEMO_UART3, menuString, sizeof(menuString) / sizeof(menuString[0]));
	UART_WriteBlocking(DEMO_UART0, optionString, sizeof(optionString) / sizeof(optionString[0]));
	UART_WriteBlocking(DEMO_UART3, optionString, sizeof(optionString) / sizeof(optionString[0]));
	UART_WriteBlocking(DEMO_UART0, readMemoryString, sizeof(readMemoryString) / sizeof(readMemoryString[0]));
	UART_WriteBlocking(DEMO_UART3, readMemoryString, sizeof(readMemoryString) / sizeof(readMemoryString[0]));
	UART_WriteBlocking(DEMO_UART0, writeMemoryString, sizeof(writeMemoryString) / sizeof(writeMemoryString[0]));
	UART_WriteBlocking(DEMO_UART3, writeMemoryString, sizeof(writeMemoryString) / sizeof(writeMemoryString[0]));
	UART_WriteBlocking(DEMO_UART0, setHourString, sizeof(setHourString) / sizeof(setHourString[0]));
	UART_WriteBlocking(DEMO_UART3, setHourString, sizeof(setHourString) / sizeof(setHourString[0]));
	UART_WriteBlocking(DEMO_UART0, setDateString, sizeof(setDateString) / sizeof(setDateString[0]));
	UART_WriteBlocking(DEMO_UART3, setDateString, sizeof(setDateString) / sizeof(setDateString[0]));
	UART_WriteBlocking(DEMO_UART0, hourFormatString, sizeof(hourFormatString) / sizeof(hourFormatString[0]));
	UART_WriteBlocking(DEMO_UART3, hourFormatString, sizeof(hourFormatString) / sizeof(hourFormatString[0]));
	UART_WriteBlocking(DEMO_UART0, readHourString, sizeof(readHourString) / sizeof(readHourString[0]));
	UART_WriteBlocking(DEMO_UART3, readHourString, sizeof(readHourString) / sizeof(readHourString[0]));
	UART_WriteBlocking(DEMO_UART0, readDateString, sizeof(readDateString) / sizeof(readDateString[0]));
	UART_WriteBlocking(DEMO_UART3, readDateString, sizeof(readDateString) / sizeof(readDateString[0]));
	UART_WriteBlocking(DEMO_UART0, chatString, sizeof(chatString) / sizeof(chatString[0]));
	UART_WriteBlocking(DEMO_UART3, chatString, sizeof(chatString) / sizeof(chatString[0]));
	UART_WriteBlocking(DEMO_UART0, ecoString, sizeof(ecoString) / sizeof(ecoString[0]));

	UART_WriteBlocking(DEMO_UART3, ecoString, sizeof(ecoString) / sizeof(ecoString[0]));
	UART_EnableInterrupts(DEMO_UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
	EnableIRQ(DEMO_UART0_RX_TX_IRQn);
	UART_EnableInterrupts(DEMO_UART3, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
	EnableIRQ(DEMO_UART3_RX_TX_IRQn);

	for(;;)
	{
		while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEMO_UART0)) && (rxIndexUart0 != txIndexUart0))
		{
			uartFlag = UART0FLAG;
			UART_WriteByte(DEMO_UART0, uart0Data);
			txIndexUart0++;
			txIndexUart0 %= 16;

			switch(optionFlag)
			{
			case OPTION_8:
				optionEightFlag = TRUE;
			}

		}
		while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEMO_UART3)) && (rxIndexUart3 != txIndexUart3))
		{
			uartFlag = UART3FLAG;
			UART_WriteByte(DEMO_UART3, uart3Data);
			txIndexUart3++;
			txIndexUart3 %= 16;

			switch(optionFlag)
			{
			case OPTION_8:
				optionEightFlag = TRUE;
			}
		}
		switch(uartFlag)
		{
		case UART0FLAG:
			if(TRUE == optionEightFlag)
			{
					CreateChatTask();
					void mainMenu_task(void *pvParameters);
					optionEightFlag = FALSE;

			}
			break;
		case UART3FLAG:
			if(TRUE == optionEightFlag)
			{
				//xTaskCreate(chat_task, "Chat_Task", configMINIMAL_STACK_SIZE, NULL,2 , chatHandle);
				break;
			}
		}
	}

}


void CreateChatTask(void)
{
	xTaskCreate(chat_task, "Chat_Task", configMINIMAL_STACK_SIZE, NULL,2 , &chatHandle);
}

TaskHandle_t optionEightHandler(){
return &chatHandle;
}
