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
uint8_t pressKeyToContinueString[] = "Presione cualquier letra para continuar\r";


/*Read Memory Menu*/
uint8_t memoryDirectionString[] = "Direccion de Lectura: 0x\r";
uint8_t lenghtToReadString[] = "Longitud en bytes:\r";
uint8_t contentString[] = "Contenido:\r";

/*Write Memory Menu*/
uint8_t memoryWriteDirectionString[] = "Direccion de Escritura:\r";
uint8_t savedTextString[] = "Texto a guardar:\r";
uint8_t savedString[] = "Su texto ha sido guardado";

/*Set hour Menu*/
uint8_t setNewHourString[] = "Escribir hora en formato hh/mm/ss:\r";
uint8_t savedHourString[] = "La hora ha sido cambiada\r";

/*Set Date Menu*/
uint8_t setNewDateString[] = "Escribir fecha en formato dd/mm/aa:\r";
uint8_t savedDateString[] = "La fecha ha sido cambiada\r";

/*Set Format Menu*/
uint8_t setNewFormatString[] = "Para cambiar el formato de hora presione 1\r";

/*Read Hour Menu*/
uint8_t readActualHourString[] = "La hora actual es:\r";

/*Read Date Menu*/
uint8_t readActualDateString[] = "La fecha actual es:\r";

/*Chat Menu*/
uint8_t showingChatString[] = "Chat:\r";
uint8_t TerminalOneEndingString[] = "Terminal se ha desconectado";

/*Eco Menu*/
uint8_t ecoLcdString[] = "Eco en pantalla LCD\r";

uint8_t keyPressedNotvalidString[] = "Tecla no valida, presiona un numero entre 0 y 9";

uint8_t adjust1[] = "\033[1;1H";
uint8_t adjust2[] = "\033[2;1H";
uint8_t adjust3[] = "\033[1;25H";
uint8_t adjust4[] = "\033[3;1H";
uint8_t adjust5[] = "\033[2;23H";
uint8_t adjust6[] = "\033[1;32H";
uint8_t adjust7[] = "\033[2;33H";
uint8_t adjust8[] = "\033[2;30H";
uint8_t adjust9[] = "\033[1;33H";
uint8_t adjut10[] = "\033[12;1H";
uint8_t deleteLine[] = "\033[2K";

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
SemaphoreHandle_t Mutex_SetHour;       //Option 3
SemaphoreHandle_t Mutex_SetDate;       //Option 4
SemaphoreHandle_t Mutex_SetFormat;     //Option 5
SemaphoreHandle_t Mutex_ReadHour;      //Option 6
SemaphoreHandle_t Mutex_ReadDate;      //Option 7
SemaphoreHandle_t Mutex_Eco;           //Option 9

void createSemaphoreMutex(void)
{
	vSemaphoreCreateBinary(NewDataUart0); /**Semaphore for indicating when new data arrives*/
	vSemaphoreCreateBinary(NewDataUart3); /**Semaphore for indicating when new data arrives*/
	xSemaphoreTake(NewDataUart0,1);
	xSemaphoreTake(NewDataUart3,1);

	Event_uartHandle0 = xEventGroupCreate();
	Event_uartHandle3 = xEventGroupCreate();

	/**Mutexs to prevent two terminals entering the same menu*/
	Mutex_ReadMem  = xSemaphoreCreateMutex();  //Option1
	Mutex_WriteMem = xSemaphoreCreateMutex();  //Option 2
	Mutex_SetHour  = xSemaphoreCreateMutex();  //Option 3
	Mutex_SetDate  = xSemaphoreCreateMutex();  //Option 4
	Mutex_SetFormat = xSemaphoreCreateMutex(); //Option 5
	Mutex_ReadHour  = xSemaphoreCreateMutex(); //Option 6
	Mutex_ReadDate  = xSemaphoreCreateMutex(); //Option 7
	Mutex_Eco       = xSemaphoreCreateMutex(); //Option 9
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
		getTime_task();
		if(!xEventGroupGetBits(Event_uartHandle0))
		{
			if(xSemaphoreTakeFromISR(NewDataUart0,pxHigherPriorityTaskWoken))
			{
				if(0x39 >= uart0Data && 0x31<= uart0Data)
				{
					switch(uart0Data)
					{
					case OPTION_1:
						if(xSemaphoreTake(Mutex_ReadMem,1))
						{
							printingReadMemMenu(DEMO_UART0);
							xEventGroupSetBits(Event_uartHandle0, MENU_OP1);
						}
						break;
					case OPTION_2:
						if(xSemaphoreTake(Mutex_WriteMem,1))
						{
							printingWriteMemMenu(DEMO_UART0);
							xEventGroupSetBits(Event_uartHandle0,MENU_OP2);
						}
						break;
					case OPTION_3:
						if(xSemaphoreTake(Mutex_SetHour,1))
						{
							printingSetHourMenu(DEMO_UART0);
							xEventGroupSetBits(Event_uartHandle0, MENU_OP3);
						}
						break;
					case OPTION_4:
						if(xSemaphoreTake(Mutex_SetDate,1))
						{
							printingSetDateMenu(DEMO_UART0);
							xEventGroupSetBits(Event_uartHandle0, MENU_OP4);
						}
						break;
					case OPTION_5:
						if(xSemaphoreTake(Mutex_SetFormat,1))
						{
							printingSetFormatMenu(DEMO_UART0);
							xEventGroupSetBits(Event_uartHandle0, MENU_OP5);
						}
						break;
					case OPTION_6:
						if(xSemaphoreTake(Mutex_ReadHour,1))
						{
							printingReadHourMenu(DEMO_UART0);
							xEventGroupSetBits(Event_uartHandle0, MENU_OP6);
						}
						break;
					case OPTION_7:
						if(xSemaphoreTake(Mutex_ReadDate,1))
						{
							printingReadDateMenu(DEMO_UART0);
							xEventGroupSetBits(Event_uartHandle0,MENU_OP7);
						}
						break;
					case OPTION_8:
						printingChatMenu(DEMO_UART0);
						xEventGroupSetBits(Event_uartHandle0,MENU_OP8);
						break;
					case OPTION_9:
						if(xSemaphoreTake(Mutex_Eco,1))
						{
							printingEcoMenu(DEMO_UART0);
							xEventGroupSetBits(Event_uartHandle0, MENU_OP9);
						}
						break;
					default:
						break;
					}
				}
				else if(uart0Data != ESCTERA)
				{
					UART_WriteBlocking(DEMO_UART0, keyPressedNotvalidString, sizeof(keyPressedNotvalidString) / sizeof(keyPressedNotvalidString[0]));
					delay(400000);
					UART_WriteBlocking(DEMO_UART0, deleteLine, sizeof(deleteLine) / sizeof(deleteLine[0]));
					UART_WriteBlocking(DEMO_UART0, adjut10, sizeof(adjut10) / sizeof(adjut10[0]));
				}
			}
		}
		else if(uart0Data == ESCTERA)
		{
			if(MENU_OP1 == xEventGroupGetBits(Event_uartHandle0))
			{
				xSemaphoreGive(Mutex_ReadMem);
				vTaskDelete(readingI2CHandle);
			}
			else if(MENU_OP2 == xEventGroupGetBits(Event_uartHandle0))
			{
				xSemaphoreGive(Mutex_WriteMem);
				vTaskDelete(writingI2CHandle);
			}
			else if(MENU_OP3 == xEventGroupGetBits(Event_uartHandle0))
			{
				xSemaphoreGive(Mutex_SetHour);
				vTaskDelete(setHourHandle);
			}
			else if(MENU_OP4 == xEventGroupGetBits(Event_uartHandle0))
			{
				xSemaphoreGive(Mutex_SetDate);
				vTaskDelete(setDateHandle);
			}
			else if(MENU_OP5 == xEventGroupGetBits(Event_uartHandle0))
			{
				xSemaphoreGive(Mutex_SetFormat);
				vTaskDelete(hourFormatHandle);
			}
			else if(MENU_OP6 == xEventGroupGetBits(Event_uartHandle0))
			{
				xSemaphoreGive(Mutex_ReadHour);
				vTaskDelete(readHourHandle);
			}
			else if(MENU_OP7 == xEventGroupGetBits(Event_uartHandle0))
			{
				xSemaphoreGive(Mutex_ReadDate);
				vTaskDelete(readDateHandle);
			}
			else if(MENU_OP8 == xEventGroupGetBits(Event_uartHandle0))
			{
				UART_WriteBlocking(DEMO_UART0, TerminalOneEndingString, sizeof(TerminalOneEndingString) / sizeof(TerminalOneEndingString[0]));
				UART_WriteBlocking(DEMO_UART0, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
				UART_WriteBlocking(DEMO_UART3, TerminalOneEndingString, sizeof(TerminalOneEndingString) / sizeof(TerminalOneEndingString[0]));
				UART_WriteBlocking(DEMO_UART3, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
				delay(450000);
				vTaskDelete(chatHandle);
			}
			else if(MENU_OP9 == xEventGroupGetBits(Event_uartHandle0))
			{
				xSemaphoreGive(Mutex_Eco);
				vTaskDelete(ecoHandle);
			}
			xEventGroupClearBits(Event_uartHandle0, MENU_OP1 | MENU_OP2 | MENU_OP3 | MENU_OP4 | MENU_OP5
					| MENU_OP6 | MENU_OP7 | MENU_OP8 | MENU_OP9);

			printingMenu(DEMO_UART0);
		}
		vTaskDelay(1);
	}
	taskYIELD();
}

void mainMenu3_task(void *pvParameters)
{
	for(;;)
	{
		if(!xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTakeFromISR(NewDataUart3,pxHigherPriorityTaskWoken))
			{
				if(0x39 >= uart3Data && 0x31<= uart3Data)
				{
					switch(uart3Data)
					{
					case OPTION_1:
						if(xSemaphoreTake(Mutex_ReadMem,1))
						{
							printingReadMemMenu(DEMO_UART3);
							xEventGroupSetBits(Event_uartHandle3, MENU_OP1);
						}
						break;
					case OPTION_2:
						if(xSemaphoreTake(Mutex_WriteMem,1))
						{
							printingWriteMemMenu(DEMO_UART3);
							xEventGroupSetBits(Event_uartHandle3,MENU_OP2);
						}
						break;
					case OPTION_3:
						if(xSemaphoreTake(Mutex_SetHour,1))
						{
							printingSetHourMenu(DEMO_UART3);
							xEventGroupSetBits(Event_uartHandle3, MENU_OP3);
						}
						break;
					case OPTION_4:
						if(xSemaphoreTake(Mutex_SetDate,1))
						{
							printingSetDateMenu(DEMO_UART3);
							xEventGroupSetBits(Event_uartHandle3, MENU_OP4);
						}
						break;
					case OPTION_5:
						if(xSemaphoreTake(Mutex_SetFormat,1))
						{
							printingSetFormatMenu(DEMO_UART3);
							xEventGroupSetBits(Event_uartHandle3, MENU_OP5);
						}
						break;
					case OPTION_6:
						if(xSemaphoreTake(Mutex_ReadHour,1))
						{
							printingReadHourMenu(DEMO_UART3);
							xEventGroupSetBits(Event_uartHandle3, MENU_OP6);
						}
						break;
					case OPTION_7:
						if(xSemaphoreTake(Mutex_ReadDate,1))
						{
							printingReadDateMenu(DEMO_UART3);
							xEventGroupSetBits(Event_uartHandle3,MENU_OP7);
						}
						break;
					case OPTION_8:
						printingChatMenu(DEMO_UART3);
						xEventGroupSetBits(Event_uartHandle3,MENU_OP8);
						break;
					case OPTION_9:
						if(xSemaphoreTake(Mutex_Eco,1))
						{
							printingEcoMenu(DEMO_UART3);
							xEventGroupSetBits(Event_uartHandle3, MENU_OP9);
						}
						break;
					default:
						break;
					}
				}
				else if(uart3Data != ESCTERA)
				{
					UART_WriteBlocking(DEMO_UART3, keyPressedNotvalidString, sizeof(keyPressedNotvalidString) / sizeof(keyPressedNotvalidString[0]));
					delay(400000);
					UART_WriteBlocking(DEMO_UART3, deleteLine, sizeof(deleteLine) / sizeof(deleteLine[0]));
					UART_WriteBlocking(DEMO_UART3, adjut10, sizeof(adjut10) / sizeof(adjut10[0]));
				}
			}
		}
		else if(uart3Data == ESCTERA)
		{
			if(MENU_OP1 == xEventGroupGetBits(Event_uartHandle3))
			{
				xSemaphoreGive(Mutex_ReadMem);
				vTaskDelete(readingI2CHandle);
			}
			else if(MENU_OP2 == xEventGroupGetBits(Event_uartHandle3))
			{
				xSemaphoreGive(Mutex_WriteMem);
				vTaskDelete(writingI2CHandle);
			}
			else if(MENU_OP3 == xEventGroupGetBits(Event_uartHandle3))
			{
				xSemaphoreGive(Mutex_SetHour);
				vTaskDelete(setHourHandle);
			}
			else if(MENU_OP4 == xEventGroupGetBits(Event_uartHandle3))
			{
				xSemaphoreGive(Mutex_SetDate);
				vTaskDelete(setDateHandle);
			}
			else if(MENU_OP5 == xEventGroupGetBits(Event_uartHandle3))
			{
				xSemaphoreGive(Mutex_SetFormat);
				vTaskDelete(hourFormatHandle);
			}
			else if(MENU_OP6 == xEventGroupGetBits(Event_uartHandle3))
			{
				xSemaphoreGive(Mutex_ReadHour);
				vTaskDelete(readHourHandle);
			}
			else if(MENU_OP7 == xEventGroupGetBits(Event_uartHandle3))
			{
				xSemaphoreGive(Mutex_ReadDate);
				vTaskDelete(readDateHandle);
			}
			else if(MENU_OP8 == xEventGroupGetBits(Event_uartHandle3))
			{
				UART_WriteBlocking(DEMO_UART0, TerminalOneEndingString, sizeof(TerminalOneEndingString) / sizeof(TerminalOneEndingString[0]));
				UART_WriteBlocking(DEMO_UART0, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
				UART_WriteBlocking(DEMO_UART3, TerminalOneEndingString, sizeof(TerminalOneEndingString) / sizeof(TerminalOneEndingString[0]));
				UART_WriteBlocking(DEMO_UART3, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
				delay(450000);
				vTaskDelete(chatHandle);
			}
			else if(MENU_OP9 == xEventGroupGetBits(Event_uartHandle3))
			{
				xSemaphoreGive(Mutex_Eco);
				vTaskDelete(ecoHandle);
			}
			xEventGroupClearBits(Event_uartHandle3, MENU_OP1 | MENU_OP2 | MENU_OP3 | MENU_OP4 | MENU_OP5
					| MENU_OP6 | MENU_OP7 | MENU_OP8 | MENU_OP9);

			printingMenu(DEMO_UART3);
		}
		vTaskDelay(1);

	}
	taskYIELD();
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
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
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
	static uint16_t readAddress;
	static uint8_t readLenght;
	static uint8 memoryCounter = 0;
	static uint8 lengthCounter = 0;
	static uint8 lengthToRead[3];
	static uint8 midatoMemoria[4]; /**address that's going to be read*/
	static uint16 numberOfBytes; /**length of the string we're reading*/
	static uint8 *readValue;
	static uint8_t received_data;
	UART_Type *currentUart;
	static uint8_t key_pressedMemory = FALSE;
	static uint8_t key_pressedLenght = FALSE;

	for(;;)
	{
		if(MENU_OP1 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP1 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTakeFromISR(NewDataUart0, pxHigherPriorityTaskWoken))
			{
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				UART_WriteByte(currentUart, received_data);
				if(ESCTERA != received_data)
				{
					key_pressedMemory = TRUE;
				}
			}
			else if(xSemaphoreTakeFromISR(NewDataUart3, pxHigherPriorityTaskWoken))
			{
				received_data = uart3Data;
				currentUart = DEMO_UART3;
				UART_WriteByte(currentUart, received_data);
				if(ESCTERA != received_data)
				{
					key_pressedLenght = TRUE;
				}
			}
			if(TRUE == key_pressedMemory)
			{
				key_pressedMemory = FALSE;
				if(memoryCounter < 4)
				{
					midatoMemoria[memoryCounter] = received_data;
					memoryCounter++;
				}
			}

			if(ENTERTERA == received_data)
			{
				memoryCounter = 0;
				key_pressedMemory = TRUE;
				UART_WriteBlocking(currentUart, adjust5, sizeof(adjust5) / sizeof(adjust5[0]));
			}
			if(TRUE == key_pressedLenght)
			{
				key_pressedLenght = FALSE;
				if(memoryCounter<3)
				{
					lengthToRead[lengthCounter] = received_data;
					lengthCounter++;
				}
			}
			if(ENTERTERA == received_data)
			{
				lengthCounter = 0;
			}
		}
		vTaskDelay(1);
		taskYIELD();
	}
}

void writingI2C_task(void *pvParameters)
{

	taskYIELD();

}

void setHour_task(void *pvParameters){
	taskYIELD();
}

void setDate_task(void *pvParameters){
	taskYIELD();
}

void hourFormat_task(void *pvParameters){
	taskYIELD();
}

void readHour_task(void *pvParameters){
	taskYIELD();
}

void readDate_task(void *pvParameters){
	taskYIELD();
}

void chat_task(void *pvParameters)
{
	static uint8_t received_data;
	UART_Type *currentUart;
	static uint8_t testBufferOne[60] = {0};
	static uint8_t testBufferFour[60] = {0};
	static uint8_t TerminalOneString[] = "Terminal 1:\r\n";
	static uint8_t TerminalTwoString[] = "Terminal 2:\r\n";
	static uint8_t bufferOneCounter = 0;
	static uint8_t bufferFourCounter = 0;
	for(;;)
	{
		if(MENU_OP8 == xEventGroupGetBits(Event_uartHandle0) && (MENU_OP8 == xEventGroupGetBits(Event_uartHandle3)))
		{
			if(xSemaphoreTakeFromISR(NewDataUart0, pxHigherPriorityTaskWoken))
			{
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				if(ENTERTERA != uart0Data)
				{
					UART_WriteByte(DEMO_UART0, uart0Data);
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
			if(xSemaphoreTakeFromISR(NewDataUart3, pxHigherPriorityTaskWoken))
			{
				received_data = uart3Data;
				currentUart = DEMO_UART3;
				if(ENTERTERA != uart3Data)
				{
					UART_WriteByte(DEMO_UART3, uart3Data);
					testBufferFour[bufferFourCounter] = uart3Data;
					bufferFourCounter++;
				}
				else if(ENTERTERA == uart3Data)
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
}

void eco_task(void *pvParameters)
{
	static uint8_t key_pressed = FALSE;
	static uint8_t received_data;
	UART_Type *currentUart;
	LCDNokia_clear();
	for(;;)
	{
		if (MENU_OP9 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP9 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTakeFromISR(NewDataUart0, pxHigherPriorityTaskWoken))
			{
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				if(ESCTERA != received_data)
				{
					UART_WriteByte(currentUart,received_data);
					LCDNokia_sendChar(received_data);
				}
			}
			else if(xSemaphoreTakeFromISR(NewDataUart3, pxHigherPriorityTaskWoken))
			{
				received_data = uart3Data;
				currentUart = DEMO_UART3;
				if(ESCTERA != received_data)
				{
					UART_WriteByte(currentUart,received_data);
					LCDNokia_sendChar(received_data);
				}
			}
		}
		vTaskDelay(1);
		taskYIELD();
	}
}


static void printingReadMemMenu(UART_Type *base)
{
	xTaskCreate(readingI2C_task, "ReadingI2C_Task", configMINIMAL_STACK_SIZE, NULL, 2, &readingI2CHandle);//OPTION 1
	if(UART0 ==  base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, memoryDirectionString, sizeof(memoryDirectionString) / sizeof(memoryDirectionString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
		UART_WriteBlocking(DEMO_UART0, lenghtToReadString, sizeof(lenghtToReadString) / sizeof(lenghtToReadString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust3, sizeof(adjust3) / sizeof(adjust3[0]));
	}
	else if(UART3 ==base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, memoryDirectionString, sizeof(memoryDirectionString) / sizeof(memoryDirectionString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
		UART_WriteBlocking(DEMO_UART3, lenghtToReadString, sizeof(lenghtToReadString) / sizeof(lenghtToReadString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust3, sizeof(adjust3) / sizeof(adjust3[0]));
	}
}

static void printingWriteMemMenu(UART_Type *base)
{
	xTaskCreate(writingI2C_task, "WritingI2C_Task", configMINIMAL_STACK_SIZE, NULL, 2, &writingI2CHandle);//OPTION 2
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, memoryWriteDirectionString, sizeof(memoryWriteDirectionString) / sizeof(memoryWriteDirectionString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
		UART_WriteBlocking(DEMO_UART0, savedTextString, sizeof(savedTextString) / sizeof(savedTextString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust4, sizeof(adjust3) / sizeof(adjust3[0]));
		UART_WriteBlocking(DEMO_UART0, adjust3, sizeof(adjust3) / sizeof(adjust3[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, memoryWriteDirectionString, sizeof(memoryWriteDirectionString) / sizeof(memoryWriteDirectionString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
		UART_WriteBlocking(DEMO_UART3, savedTextString, sizeof(savedTextString) / sizeof(savedTextString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust4, sizeof(adjust3) / sizeof(adjust3[0]));
		UART_WriteBlocking(DEMO_UART3, adjust3, sizeof(adjust3) / sizeof(adjust3[0]));
	}
}

static void printingSetHourMenu(UART_Type *base)
{
	xTaskCreate(setHour_task, "SetHour_Task", configMINIMAL_STACK_SIZE, NULL, 2, &setHourHandle);//OPTION 3
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, setNewHourString, sizeof(setNewHourString) / sizeof(setNewHourString[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, setNewHourString, sizeof(setNewHourString) / sizeof(setNewHourString[0]));
	}
}

static void printingSetDateMenu(UART_Type *base)
{
	xTaskCreate(setDate_task, "SetDate_Task", configMINIMAL_STACK_SIZE, NULL, 2, &setDateHandle);//OPTION 4
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, setNewDateString, sizeof(setNewDateString) / sizeof(setNewDateString[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, setNewDateString, sizeof(setNewDateString) / sizeof(setNewDateString[0]));
	}
}

static void printingSetFormatMenu(UART_Type *base)
{
	xTaskCreate(hourFormat_task, "HourFormat", configMINIMAL_STACK_SIZE, NULL,2, &hourFormatHandle);//OPTION 5
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, setNewFormatString, sizeof(setNewFormatString) / sizeof(setNewFormatString[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, setNewFormatString, sizeof(setNewFormatString) / sizeof(setNewFormatString[0]));
	}
}

static void printingReadHourMenu(UART_Type *base)
{
	xTaskCreate(readHour_task, "ReadHour_Task", configMINIMAL_STACK_SIZE, NULL, 2, &readHourHandle); //OPTION 6
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, readActualHourString, sizeof(readActualHourString) / sizeof(readActualHourString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, readActualHourString, sizeof(readActualHourString) / sizeof(readActualHourString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
}

static void printingReadDateMenu(UART_Type *base)
{
	xTaskCreate(readDate_task, "ReadDate_Task", configMINIMAL_STACK_SIZE, NULL, 2, &readDateHandle);//OPTION 7
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, readActualDateString, sizeof(readActualDateString) / sizeof(readActualDateString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, readActualDateString, sizeof(readActualDateString) / sizeof(readActualDateString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
}

static void printingChatMenu(UART_Type *base)
{
	xTaskCreate(chat_task, "Chat_Task", configMINIMAL_STACK_SIZE, NULL, 2, &chatHandle);//OPTION 8
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, showingChatString, sizeof(showingChatString) / sizeof(showingChatString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, showingChatString, sizeof(showingChatString) / sizeof(showingChatString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
}

static void printingEcoMenu(UART_Type *base)
{
	xTaskCreate(eco_task, "Eco_Task", configMINIMAL_STACK_SIZE, NULL, 2, &ecoHandle);// OPTION 9
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, ecoLcdString, sizeof(ecoLcdString) / sizeof(ecoLcdString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, ecoLcdString, sizeof(ecoLcdString) / sizeof(ecoLcdString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
}
