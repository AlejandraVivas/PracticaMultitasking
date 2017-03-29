/*
 * SerialMenu.c
 *
 *  Created on: 19/03/2017
 *      Author: Alejandra Meza
 */

#include "SerialMenu.h"

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
uint8_t NewLineString[] = "\r\n";

uint8_t uart0Data;
uint8_t uart3Data;

uint8_t uartFlag;

TaskHandle_t readingI2CHandle = NULL;
TaskHandle_t writingI2CHandle = NULL;
TaskHandle_t setHourHandle = NULL;
TaskHandle_t setDateHandle = NULL;
TaskHandle_t hourFormatHandle = NULL;
TaskHandle_t readHourHandle = NULL;
TaskHandle_t readDateHandle = NULL;
TaskHandle_t chatHandle = NULL;
TaskHandle_t ecoHandle = NULL;

uint8_t demoRingBufferUart0[16];
volatile uint16_t txIndexUart0; /* Index of the data to send out. */
volatile uint16_t rxIndexUart0; /* Index of the memory to save new arrived data. */

uint8_t demoRingBufferUart3[16];
volatile uint16_t txIndexUart3; /* Index of the data to send out. */
volatile uint16_t rxIndexUart3; /* Index of the memory to save new arrived data. */

BaseType_t *pxHigherPriorityTaskWoken; /**For receiving data*/
BaseType_t *PriorityTaskWoken_4;

EventGroupHandle_t Event_uartHandle0; /**Handler for menu options*/
EventGroupHandle_t Event_uartHandle3; /**Handler for menu options of UART4*/

EventGroupHandle_t NewDataUart0; /**Indicates that new data has arrived*///UART0
EventGroupHandle_t NewDataUart3; /**Indicates that new data has arrived*///UART4//


SemaphoreHandle_t Mutex_ReadMem;       //Option 1
SemaphoreHandle_t Mutex_WriteMem;      //Option 2
SemaphoreHandle_t Mutex_SetTime;       //Option 3
SemaphoreHandle_t Mutex_SetDate;       //Option 4
SemaphoreHandle_t Mutex_SetFormat;     //Option 5
SemaphoreHandle_t Mutex_ReadTime;      //Option 6
SemaphoreHandle_t Mutex_ReadDate;      //Option 7
SemaphoreHandle_t Mutex_SPI;           //Option 9

void createSemaphoreMutex(void)
{
	vSemaphoreCreateBinary(NewDataUart0); /**Semaphore for indicating when new data arrives*/
	vSemaphoreCreateBinary(NewDataUart3); /**Semaphore for indicating when new data arrives*/
	xSemaphoreTake(NewDataUart0,1);
	xSemaphoreTake(NewDataUart3,1);

	Event_uartHandle0 = xEventGroupCreate();
	Event_uartHandle3 = xEventGroupCreate();

	/**Mutexs to prevent two terminals entering the same menu*/
	Mutex_ReadMem  = xSemaphoreCreateMutex();
	Mutex_WriteMem = xSemaphoreCreateMutex();  //Option 2
	Mutex_SetTime  = xSemaphoreCreateMutex();  //Option 3
	Mutex_SetDate  = xSemaphoreCreateMutex();  //Option 4
	Mutex_SetFormat = xSemaphoreCreateMutex(); //Option 5
	Mutex_ReadTime  = xSemaphoreCreateMutex(); //Option 6
	Mutex_ReadDate  = xSemaphoreCreateMutex(); //Option 7
	Mutex_SPI       = xSemaphoreCreateMutex(); //Option 9
}

void DEMO_UART0_IRQHandler(void)
{
	/* If new data arrived. */
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(DEMO_UART0))
	{
		uart0Data = UART_ReadByte(DEMO_UART0);
		if (((rxIndexUart0 + 1) % 16) != txIndexUart0)
		{
			demoRingBufferUart0[rxIndexUart0] = uart0Data;
			rxIndexUart0++;
			rxIndexUart0 %= 16;
		}
		xSemaphoreGiveFromISR(NewDataUart0, pxHigherPriorityTaskWoken);
	}
}

void DEMO_UART3_IRQHandler(void)
{

	/* If new data arrived. */
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(DEMO_UART3))
	{
		uart3Data = UART_ReadByte(DEMO_UART3);
		if (((rxIndexUart3 + 1) % 16) != txIndexUart3)
		{
			demoRingBufferUart3[rxIndexUart3] = uart0Data;
			rxIndexUart3++;
			rxIndexUart3 %= 16;
		}
		xSemaphoreGiveFromISR(NewDataUart3, pxHigherPriorityTaskWoken);
	}
}





void mainMenu0_task(void *pvParameters)
{
	for(;;)
	{
		if(!xEventGroupGetBits(Event_uartHandle0))
		{
			if(xSemaphoreTakeFromISR(NewDataUart0,pxHigherPriorityTaskWoken))
			{
				switch(uart0Data)
				{
				case OPTION_1:
					if(xSemaphoreTake(Mutex_ReadMem,1))
					{

					}
				}
			}
		}
	}
}


void mainMenu3_task(void *pvParameters)
{
	for(;;)
	{

	}
}

void printingMenu(UART_Type *base)
{
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, menuString, sizeof(menuString) / sizeof(menuString[0]));
		UART_WriteBlocking(DEMO_UART0, optionString, sizeof(optionString) / sizeof(optionString[0]));
		UART_WriteBlocking(DEMO_UART0, readMemoryString, sizeof(readMemoryString) / sizeof(readMemoryString[0]));
		UART_WriteBlocking(DEMO_UART0, writeMemoryString, sizeof(writeMemoryString) / sizeof(writeMemoryString[0]));
		UART_WriteBlocking(DEMO_UART0, setHourString, sizeof(setHourString) / sizeof(setHourString[0]));
		UART_WriteBlocking(DEMO_UART0, setDateString, sizeof(setDateString) / sizeof(setDateString[0]));
		UART_WriteBlocking(DEMO_UART0, hourFormatString, sizeof(hourFormatString) / sizeof(hourFormatString[0]));
		UART_WriteBlocking(DEMO_UART0, readHourString, sizeof(readHourString) / sizeof(readHourString[0]));
		UART_WriteBlocking(DEMO_UART0, readDateString, sizeof(readDateString) / sizeof(readDateString[0]));
		UART_WriteBlocking(DEMO_UART0, chatString, sizeof(chatString) / sizeof(chatString[0]));
		UART_WriteBlocking(DEMO_UART0, ecoString, sizeof(ecoString) / sizeof(ecoString[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, menuString, sizeof(menuString) / sizeof(menuString[0]));
		UART_WriteBlocking(DEMO_UART3, optionString, sizeof(optionString) / sizeof(optionString[0]));
		UART_WriteBlocking(DEMO_UART3, readMemoryString, sizeof(readMemoryString) / sizeof(readMemoryString[0]));
		UART_WriteBlocking(DEMO_UART3, writeMemoryString, sizeof(writeMemoryString) / sizeof(writeMemoryString[0]));
		UART_WriteBlocking(DEMO_UART3, setHourString, sizeof(setHourString) / sizeof(setHourString[0]));
		UART_WriteBlocking(DEMO_UART3, setDateString, sizeof(setDateString) / sizeof(setDateString[0]));
		UART_WriteBlocking(DEMO_UART3, hourFormatString, sizeof(hourFormatString) / sizeof(hourFormatString[0]));
		UART_WriteBlocking(DEMO_UART3, readHourString, sizeof(readHourString) / sizeof(readHourString[0]));
		UART_WriteBlocking(DEMO_UART3, readDateString, sizeof(readDateString) / sizeof(readDateString[0]));
		UART_WriteBlocking(DEMO_UART3, chatString, sizeof(chatString) / sizeof(chatString[0]));
		UART_WriteBlocking(DEMO_UART3, ecoString, sizeof(ecoString) / sizeof(ecoString[0]));
	}
}



void readingI2C_task(void *pvParameters)
{

	static uint8 memoryCounter = 0;
	static uint8 lengthCounter = 0;
	static uint8 lengthToRead[3];
	static uint8 midatoMemoria[4]; /**address that's going to be read*/
	//static uint16 counterValue = OPTION;
	static uint16 numberOfBytes; /**length of the string we're reading*/
	static uint8 *readValue;

	uint8_t memoryDirectionString[] = "Direccion de Lectura: ";
	uint8_t lenghtToReadString[] = "Longitud en bytes: ";
	uint8_t contentString[] = "Contenido: ";
	for(;;)
	{
		if(uartFlag ==  UART0FLAG)
		{
			UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
			UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
			UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
			UART_WriteBlocking(DEMO_UART0, memoryDirectionString, sizeof(memoryDirectionString) / sizeof(memoryDirectionString[0]));
			while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEMO_UART0)) && (rxIndexUart0 != txIndexUart0))
			{
				UART_WriteByte(DEMO_UART0, uart0Data);
				txIndexUart0++;
				txIndexUart0 %= 16;
				for(memoryCounter = 0; memoryCounter <= 4; memoryCounter++)
				{

				}

			}
		}
		else if(uartFlag == UART3FLAG)
		{

		}
	}
}

void writingI2C_task(void *pvParameters)
{

	PRINTF("Hola escribir i2c");

}

void setHour_task(void *pvParameters){

}

void setDate_task(void *pvParameters){

}

void hourFormat_task(void *pvParameters){

}

void readHour_task(void *pvParameters){

}

void readDate_task(void *pvParameters){

}

void chat_task(void *pvParameters)
{
	uint8_t testBufferOne[60] = {0};
	uint8_t testBufferFour[60] = {0};
	uint8_t TerminalOneString[] = "Terminal 1:\r\n";
	uint8_t TerminalTwoString[] = "Terminal 2:\r\n";
	uint8_t TerminalOneEndingString[] = "Terminal 1 se ha desconectado";
	uint8_t TerminalTwoEndingString[] = "Terminal 2 se ha desconectado";
	uint8_t bufferOneCounter = 0;
	uint8_t bufferFourCounter = 0;

	UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
	UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
	UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));

	for(;;)
	{
		if(uartFlag == UART0FLAG)
		{
			if((ESCTERA != uart0Data)  /*|| (ESCBLUE != uart3Data)*/)
			{
				while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEMO_UART0)) && (rxIndexUart0 != txIndexUart0))
				{
					UART_WriteByte(DEMO_UART0, uart0Data);
					txIndexUart0++;
					txIndexUart0 %= 16;
					if(ENTERTERA !=  uart0Data)
					{
						testBufferOne[bufferOneCounter] = uart0Data;
						bufferOneCounter++;
					}
					else if(ENTERTERA == uart0Data)
					{
						bufferOneCounter = 0;
						UART_WriteBlocking(DEMO_UART0, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
						UART_WriteBlocking(DEMO_UART0, TerminalOneString, sizeof(TerminalOneString) / sizeof(TerminalOneString[0]));
						UART_WriteBlocking(DEMO_UART0, testBufferOne, sizeof(testBufferOne) / sizeof(testBufferOne[0]));
						UART_WriteBlocking(DEMO_UART0, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
						UART_WriteBlocking(DEMO_UART3, TerminalOneString, sizeof(TerminalOneString) / sizeof(TerminalOneString[0]));
						UART_WriteBlocking(DEMO_UART3, testBufferOne, sizeof(testBufferOne) / sizeof(testBufferOne[0]));
						UART_WriteBlocking(DEMO_UART3, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
					}
				}
			}
		}
		else if(uartFlag == UART3FLAG)
		{
			if((ESCBLUE != uart3Data))
			{
				while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEMO_UART3)) && (rxIndexUart3 != txIndexUart3))
				{
					UART_WriteByte(DEMO_UART3, uart3Data);
					txIndexUart3++;
					txIndexUart3 %= 16;

					if(ENTERBLUE !=  uart3Data)
					{
						testBufferFour[bufferFourCounter] = uart3Data;
						bufferFourCounter++;
					}
					else if(ENTERBLUE == uart3Data) //ENTERBLUE = [
					{
						bufferFourCounter = 0;
						UART_WriteBlocking(DEMO_UART3, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
						UART_WriteBlocking(DEMO_UART3, TerminalTwoString, sizeof(TerminalTwoString) / sizeof(TerminalTwoString[0]));
						UART_WriteBlocking(DEMO_UART3, testBufferFour, sizeof(testBufferFour) / sizeof(testBufferFour[0]));
						UART_WriteBlocking(DEMO_UART3, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
						UART_WriteBlocking(DEMO_UART0, TerminalTwoString, sizeof(TerminalTwoString) / sizeof(TerminalTwoString[0]));
						UART_WriteBlocking(DEMO_UART0, testBufferFour, sizeof(testBufferFour) / sizeof(testBufferFour[0]));
						UART_WriteBlocking(DEMO_UART0, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
					}
				}
			}
		}
		if((ESCTERA == uart0Data)) //ESCBLUE = ]
		{
			UART_WriteBlocking(DEMO_UART0, TerminalOneEndingString, sizeof(TerminalOneEndingString) / sizeof(TerminalOneEndingString[0]));
			UART_WriteBlocking(DEMO_UART0, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
			UART_WriteBlocking(DEMO_UART3, TerminalOneEndingString, sizeof(TerminalOneEndingString) / sizeof(TerminalOneEndingString[0]));
			UART_WriteBlocking(DEMO_UART3, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
			delay(450000);
			vTaskDelete(chatHandle);
		}
		else if((ESCBLUE == uart3Data))
		{
			UART_WriteBlocking(DEMO_UART3, TerminalTwoEndingString, sizeof(TerminalTwoEndingString) / sizeof(TerminalTwoEndingString[0]));
			UART_WriteBlocking(DEMO_UART3, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
			UART_WriteBlocking(DEMO_UART0, TerminalTwoEndingString, sizeof(TerminalTwoEndingString) / sizeof(TerminalTwoEndingString[0]));
			UART_WriteBlocking(DEMO_UART0, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
			delay(450000);
			vTaskDelete(chatHandle);
		}
		taskYIELD();
	}
}

void eco_task(void *pvParameters)
{
	if(uartFlag == UART0FLAG)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
	}
	LCDNokia_clear();
	for(;;)
	{
		if(uartFlag == UART0FLAG)
		{
			if(ESCTERA != uart0Data)
			{
				while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEMO_UART0)) && (rxIndexUart0 != txIndexUart0))
				{
					UART_WriteByte(DEMO_UART0, uart0Data);
					txIndexUart0++;
					txIndexUart0 %= 16;

					LCDNokia_sendChar(uart0Data);
				}
			}
			else if(ESCTERA == uart0Data)
			{
				LCDNokia_clear();
				vTaskDelete(ecoHandle);
			}
		}
		else if(uartFlag == UART3FLAG)
		{
			if(ESCBLUE != uart3Data)
			{
				while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEMO_UART3)) && (rxIndexUart3 != txIndexUart3))
				{
					UART_WriteByte(DEMO_UART3, uart3Data);
					txIndexUart3++;
					txIndexUart3 %= 16;
					LCDNokia_sendChar(uart3Data);
				}
			}
			else if(ESCBLUE == uart3Data)
			{
				LCDNokia_clear();
				vTaskDelete(ecoHandle);
			}
		}
	}
}



