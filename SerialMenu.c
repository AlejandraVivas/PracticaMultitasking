/*
 * SerialMenu.c
 *
 *  Created on: 19/03/2017
 *      Author: Alejandra Meza
 */

#include "SerialMenu.h"

uint8_t buttonOneFlag = FALSE;
uint8_t buttonTwoFlag = FALSE;
uint8_t buttonThreeFlag = FALSE;
uint8_t buttonFourFlag = FALSE;
uint8_t buttonFiveFlag = FALSE;
uint8_t buttonSixFlag = FALSE;

uint32_t bitNumber = 0;

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
uint8_t lenghtToWriteString[] = "Longitud en bytes: \r";
uint8_t savedTextString[] = "Texto a guardar:\r";
uint8_t savedString[] = "Su texto ha sido guardado";

/*Set hour Menu*/
uint8_t setNewHourString[] = "Escribir hora en formato hh/mm:\r";
uint8_t savedHourString[] = "La hora ha sido cambiada\r";

/*Set Date Menu*/
uint8_t setNewDateString[] = "Escribir fecha en formato dd/mm/aa:\r";
uint8_t savedDateString[] = "La fecha ha sido cambiada\r";

/*Set Format Menu*/
uint8_t setNewFormatString[] = "Cambio de formato de hora\r";
uint8_t setNewFormatOptionString[] = "Para Formato 12 hrs presione 1, para formato de 24 hrs presiona 2: \r";

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

uint8_t pointsString[] = ":";
uint8_t lineString[] = "-";
uint8_t twentyString[] = "-20";

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
uint8_t adjust11[] = "\033[4;1H";
uint8_t adjust12[] = "\033[3;18H";
uint8_t deleteLine[] = "\033[2K";


uint8_t errorString[] = "error";

uint8_t uart0Data;
uint8_t uart3Data;

uint8_t readHourUart0Flag;
uint8_t readHourUart3Flag;

uint8_t readDateUart0Flag;
uint8_t readDateUart3Flag;


TaskHandle_t readingI2CHandle = NULL;
TaskHandle_t writingI2CHandle = NULL;
TaskHandle_t setHourHandle = NULL;
TaskHandle_t setDateHandle = NULL;
TaskHandle_t hourFormatHandle = NULL;
TaskHandle_t readHourHandle = NULL;
TaskHandle_t readDateHandle = NULL;
TaskHandle_t chatHandle = NULL;
TaskHandle_t ecoHandle = NULL;
TaskHandle_t setNewDateHandle = NULL;
extern TaskHandle_t lcdHandle;


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

SemaphoreHandle_t xCountingSemaphore;

BaseType_t xHigherPriorityTaskWoken = pdFALSE;

/*get time*/
uint16_t hexAddress;
uint8_t counter = 0;
uint8_t timeBuffer[7];
uint8_t qCounter=0;
uint8_t asciiDate[12];
uint8_t *ptrToDate = asciiDate;
uint8_t HOURS_REG_SIZE = 0x7F;
uint8_t myRtcDataTime[5];
uint8_t myRtcDataDate[5];
/****/

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

	//xCountingSemaphore = xSemaphoreCreateCounting(10,0);

}


//void PORTC_IRQHandler(void)
//{
//	bitNumber = GPIO_GetPinsInterruptFlags(GPIOC);
//	switch(bitNumber)
//	{
//	case BIT5:
//		GPIO_ClearPinsInterruptFlags(GPIOC, BIT5);
//		buttonOneFlag = TRUE;
//		break;
//	case BIT7:
//		GPIO_ClearPinsInterruptFlags(GPIOC, BIT7);
//		buttonTwoFlag = TRUE;
//		break;
//	case BIT0:
//		GPIO_ClearPinsInterruptFlags(GPIOC, BIT0);
//		buttonThreeFlag = TRUE;
//		break;
//	case BIT9:
//		GPIO_ClearPinsInterruptFlags(GPIOC, BIT9);
//		buttonFourFlag = TRUE;
//		break;
//	default:
//		break;
//	}
//	xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );
//	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//}


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
			if(xSemaphoreTake(NewDataUart0,portMAX_DELAY))
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
							taskENTER_CRITICAL();
							readHourUart0Flag = 1;
							taskEXIT_CRITICAL();
							printingReadHourMenu(DEMO_UART0);
							xEventGroupSetBits(Event_uartHandle0, MENU_OP6);
						}
						break;
					case OPTION_7:
						if(xSemaphoreTake(Mutex_ReadDate,1))
						{
							taskENTER_CRITICAL();
							readDateUart0Flag = 1;
							taskEXIT_CRITICAL();
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
			else if(MENU_OP9 == xEventGroupGetBits(Event_uartHandle0))
			{
				xSemaphoreGive(Mutex_Eco);
				vTaskDelete(ecoHandle);
				xTaskCreate(serialTimeLCD, "serialTimeLCD", configMINIMAL_STACK_SIZE, NULL, 3,&lcdHandle);
			}
			xEventGroupClearBits(Event_uartHandle0, MENU_OP1 | MENU_OP2 | MENU_OP3 | MENU_OP4 | MENU_OP5
					| MENU_OP6 | MENU_OP7 | MENU_OP8 | MENU_OP9);

			printingMenu(DEMO_UART0);
		}
		vTaskDelay(1);
		taskYIELD();
	}
}

void mainMenu3_task(void *pvParameters)
{
	for(;;)
	{
		if(!xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTake(NewDataUart3,portMAX_DELAY))
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
							taskENTER_CRITICAL();
							readHourUart3Flag = 1;
							taskEXIT_CRITICAL();
							printingReadHourMenu(DEMO_UART3);
							xEventGroupSetBits(Event_uartHandle3, MENU_OP6);
						}
						break;
					case OPTION_7:
						if(xSemaphoreTake(Mutex_ReadDate,1))
						{
							taskENTER_CRITICAL();
							readDateUart3Flag = 1;
							taskEXIT_CRITICAL();
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
			else if(MENU_OP9 == xEventGroupGetBits(Event_uartHandle3))
			{
				xSemaphoreGive(Mutex_Eco);
				vTaskDelete(ecoHandle);
				xTaskCreate(serialTimeLCD, "serialTimeLCD", configMINIMAL_STACK_SIZE, NULL, 3, &lcdHandle);
			}
			xEventGroupClearBits(Event_uartHandle3, MENU_OP1 | MENU_OP2 | MENU_OP3 | MENU_OP4 | MENU_OP5
					| MENU_OP6 | MENU_OP7 | MENU_OP8 | MENU_OP9);
			printingMenu(DEMO_UART3);
		}
		vTaskDelay(1);
		taskYIELD();

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
		UART_WriteBlocking(DEMO_UART0, deleteLine, sizeof(deleteLine) / sizeof(deleteLine[0]));
		UART_WriteBlocking(DEMO_UART0, adjut10, sizeof(adjut10) / sizeof(adjut10[0]));
		UART_WriteBlocking(DEMO_UART0, deleteLine, sizeof(deleteLine) / sizeof(deleteLine[0]));
		UART_WriteBlocking(DEMO_UART0, adjut10, sizeof(adjut10) / sizeof(adjut10[0]));
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
		UART_WriteBlocking(DEMO_UART3, deleteLine, sizeof(deleteLine) / sizeof(deleteLine[0]));
		UART_WriteBlocking(DEMO_UART3, adjut10, sizeof(adjut10) / sizeof(adjut10[0]));
		UART_WriteBlocking(DEMO_UART3, deleteLine, sizeof(deleteLine) / sizeof(deleteLine[0]));
		UART_WriteBlocking(DEMO_UART3, adjut10, sizeof(adjut10) / sizeof(adjut10[0]));
	}
}



void readingI2C_task(void *pvParameters)
{
	static uint16_t readAddress;
	static uint32_t readLenght;
	uint8_t testAddress[4];
	uint8_t memoryCounter = 0;
	static uint8_t lengthToRead[3];
	static uint8_t midatoMemoria[7]; /**address that's going to be read*/
	uint8_t readValue[255];
	static uint8_t received_data;
	UART_Type *currentUart;
	uint8_t key_pressedMemory = TRUE;
	for(;;)
	{
		if(MENU_OP1 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP1 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTake(NewDataUart0, portMAX_DELAY))
			{
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				UART_WriteByte(currentUart, received_data);

				if(TRUE == key_pressedMemory)
				{

					midatoMemoria[memoryCounter] = received_data;
					memoryCounter++;

					if(memoryCounter == 4)
					{
						testAddress[0] = midatoMemoria[0];
						testAddress[1] = midatoMemoria[1];
						testAddress[2] = midatoMemoria[2];
						testAddress[3] = midatoMemoria[3];
						UART_WriteBlocking(currentUart, adjust5, sizeof(adjust5) / sizeof(adjust5[0]));
					}
					else if(memoryCounter == 7)
					{
						lengthToRead[0] = midatoMemoria[4];
						lengthToRead[1] = midatoMemoria[5];
						lengthToRead[2] = midatoMemoria[6];
					}
				}
				if(ENTERTERA == received_data)
				{
					key_pressedMemory = FALSE;
					readAddress = asciiToHex(testAddress);
					readLenght = asciiToHex(lengthToRead);

					I2C_MemoryRead(I2C0, MEM_DEVICE_ADD, readAddress,readValue, readLenght);
					UART_WriteBlocking(currentUart, adjust4, sizeof(adjust4) / sizeof(adjust4[0]));
					UART_WriteBlocking(currentUart, contentString, sizeof(contentString) / sizeof(contentString[0]));
					UART_WriteBlocking(currentUart, adjust11, sizeof(adjust11) / sizeof(adjust11[0]));
					UART_WriteBlocking(currentUart, readValue,readLenght);

				}

			}
			else if(xSemaphoreTake(NewDataUart3, portMAX_DELAY))
			{
				received_data = uart3Data;
				currentUart = DEMO_UART3;
				UART_WriteByte(currentUart, received_data);

				if(TRUE == key_pressedMemory)
				{

					midatoMemoria[memoryCounter] = received_data;
					memoryCounter++;

					if(memoryCounter == 4)
					{
						testAddress[0] = midatoMemoria[0];
						testAddress[1] = midatoMemoria[1];
						testAddress[2] = midatoMemoria[2];
						testAddress[3] = midatoMemoria[3];
						UART_WriteBlocking(currentUart, adjust5, sizeof(adjust5) / sizeof(adjust5[0]));
					}
					else if(memoryCounter == 7)
					{
						lengthToRead[0] = midatoMemoria[4];
						lengthToRead[1] = midatoMemoria[5];
						lengthToRead[2] = midatoMemoria[6];
					}
				}
				if(ENTERTERA == received_data)
				{
					key_pressedMemory = FALSE;
					readAddress = asciiToHex(testAddress);
					readLenght = asciiToHex(lengthToRead);

					I2C_MemoryRead(I2C0, MEM_DEVICE_ADD, readAddress,readValue, readLenght);
					UART_WriteBlocking(currentUart, adjust4, sizeof(adjust4) / sizeof(adjust4[0]));
					UART_WriteBlocking(currentUart, contentString, sizeof(contentString) / sizeof(contentString[0]));
					UART_WriteBlocking(currentUart, adjust11, sizeof(adjust11) / sizeof(adjust11[0]));
					UART_WriteBlocking(currentUart, readValue, sizeof(readValue) / sizeof(readValue[0]));
				}

			}
		}
		vTaskDelay(1);
		taskYIELD();
	}
}

void writingI2C_task(void *pvParameters)
{
	static uint16_t readAddress;
	static uint32_t readLenght;
	uint8_t data[200];
	uint8_t dataIn[7];
	static uint8_t dataLong[3];
	static uint8_t writeDirection[4];
	static uint8_t received_data;
	UART_Type *currentUart;
	uint8_t counter = 0;
	uint8_t dataCounter = 0;
	uint8_t key_pressedMemory = TRUE;
	uint8_t key_pressedDataIn = FALSE;
	for(;;)
	{
		if(MENU_OP2 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP2 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTake(NewDataUart0, portMAX_DELAY))
			{
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				UART_WriteByte(currentUart, received_data);

				if(TRUE == key_pressedDataIn)
				{
					if(ENTERTERA != received_data)
					{
						data[dataCounter] = received_data;
						dataCounter++;
					}
					else if(ENTERTERA == received_data)
					{
						dataCounter = 0;
						delay(100);
						I2C_MemoryWrite(I2C0, MEM_DEVICE_ADD, readAddress, data, readLenght);
						key_pressedDataIn = 0;
					}
				}

				else if(TRUE == key_pressedMemory)
				{
					if(ENTERTERA != received_data)
					{
						dataIn[counter] = received_data;
						counter++;

						if(counter == 4)
						{
							writeDirection[0] = dataIn[0];
							writeDirection[1] = dataIn[1];
							writeDirection[2] = dataIn[2];
							writeDirection[3] = dataIn[3];

							readAddress = asciiToHex(writeDirection);
							UART_WriteBlocking(currentUart, adjust5, sizeof(adjust5) / sizeof(adjust5[0]));
						}
					}
					else if(ENTERTERA == received_data)
					{
						counter = 0;

						UART_WriteBlocking(currentUart, adjust11, sizeof(adjust11) / sizeof(adjust11[0]));
						dataLong[0] = dataIn[4];
						dataLong[1] = dataIn[5];
						dataLong[2] = dataIn[6];

						readLenght =  asciiToHex(dataLong);
						key_pressedMemory = FALSE;
						key_pressedDataIn = TRUE;
					}
				}
			}
			else if(xSemaphoreTake(NewDataUart3, portMAX_DELAY))
			{
				received_data = uart3Data;
				currentUart = DEMO_UART3;
				UART_WriteByte(currentUart, received_data);

				if(TRUE == key_pressedDataIn)
				{
					if(ENTERTERA != received_data)
					{
						data[dataCounter] = received_data;
						dataCounter++;
					}
					else if(ENTERTERA == received_data)
					{
						dataCounter = 0;
						delay(100);
						I2C_MemoryWrite(I2C0, MEM_DEVICE_ADD, readAddress, data, readLenght);
						key_pressedDataIn = 0;
					}
				}

				else if(TRUE == key_pressedMemory)
				{
					if(ENTERTERA != received_data)
					{
						dataIn[counter] = received_data;
						counter++;

						if(counter == 4)
						{
							writeDirection[0] = dataIn[0];
							writeDirection[1] = dataIn[1];
							writeDirection[2] = dataIn[2];
							writeDirection[3] = dataIn[3];

							readAddress = asciiToHex(writeDirection);
							UART_WriteBlocking(currentUart, adjust5, sizeof(adjust5) / sizeof(adjust5[0]));
						}
					}
					else if(ENTERTERA == received_data)
					{
						counter = 0;

						UART_WriteBlocking(currentUart, adjust11, sizeof(adjust11) / sizeof(adjust11[0]));
						dataLong[0] = dataIn[4];
						dataLong[1] = dataIn[5];
						dataLong[2] = dataIn[6];

						readLenght =  asciiToHex(dataLong);
						key_pressedMemory = FALSE;
						key_pressedDataIn = TRUE;
					}
				}
			}
		}
		vTaskDelay(10);
		taskYIELD();
	}
}

void setHour_task(void *pvParameters)
{
	static uint8_t received_data;
	UART_Type *currentUart;
	static uint8_t counter = 0;
	static uint8_t myRtcData[5];
	for(;;)
	{
		if(MENU_OP3 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP3 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTake(NewDataUart0, portMAX_DELAY))
			{
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				if(ENTERTERA != received_data)
				{
					UART_WriteByte(currentUart, received_data);
					myRtcDataTime[counter] = received_data;
					counter++;
				}
				else if(ENTERTERA == received_data)
				{
					counter = 0;
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x02, ((myRtcDataTime[0]-ASCII_NUMBER_MASK))<<4 | (myRtcDataTime[1]-ASCII_NUMBER_MASK));
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x01, ((myRtcDataTime[2]-ASCII_NUMBER_MASK))<<4 | (myRtcDataTime[3]-ASCII_NUMBER_MASK));
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD,0x00, (0x80 | ((myRtcDataTime[4]-ASCII_NUMBER_MASK))<<4 | (myRtcDataTime[5]-ASCII_NUMBER_MASK)));
				}
			}
			else if(xSemaphoreTake(NewDataUart3, portMAX_DELAY))
			{
				received_data = uart3Data;
				currentUart = DEMO_UART3;
				if(ENTERTERA != received_data)
				{
					UART_WriteByte(currentUart, received_data);
					myRtcDataTime[counter] = received_data;
					counter++;
				}
				else if(ENTERTERA == received_data)
				{
					counter = 0;
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x02, ((myRtcDataTime[0]-ASCII_NUMBER_MASK))<<4 | (myRtcDataTime[1]-ASCII_NUMBER_MASK));
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x01, ((myRtcDataTime[2]-ASCII_NUMBER_MASK))<<4 | (myRtcDataTime[3]-ASCII_NUMBER_MASK));
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD,0x00, (0x80 | ((myRtcDataTime[4]-ASCII_NUMBER_MASK))<<4 | (myRtcDataTime[5]-ASCII_NUMBER_MASK)));
				}
			}


		}
		vTaskDelay(1);
		taskYIELD();
	}
}

void setDate_task(void *pvParameters)
{
	static uint8_t received_data;
	UART_Type *currentUart;
	static uint8_t counter = 0;
	for(;;)
	{
		if(MENU_OP4 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP4 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTake(NewDataUart0, portMAX_DELAY))
			{
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				if(ENTERTERA != received_data)
				{
					UART_WriteByte(currentUart, received_data);
					myRtcDataDate[counter] = received_data;
					counter++;
				}
				else if(ENTERTERA == received_data)
				{
					counter = 0;

					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x04, ((myRtcDataDate[0]-ASCII_NUMBER_MASK))<<4 | (myRtcDataDate[1]-ASCII_NUMBER_MASK));
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x05, ((myRtcDataDate[2]-ASCII_NUMBER_MASK))<<4 | (myRtcDataDate[3]-ASCII_NUMBER_MASK));
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD,0x06, ((myRtcDataDate[4]-ASCII_NUMBER_MASK))<<4 | (myRtcDataDate[5]-ASCII_NUMBER_MASK));
				}
			}
			else if(xSemaphoreTake(NewDataUart3, portMAX_DELAY))
			{
				received_data = uart3Data;
				currentUart = DEMO_UART3;
				if(ENTERTERA != received_data)
				{
					UART_WriteByte(currentUart, received_data);
					myRtcDataDate[counter] = received_data;
					counter++;
				}
				else if(ENTERTERA == received_data)
				{
					counter = 0;
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x04, ((myRtcDataDate[0]-ASCII_NUMBER_MASK))<<4 | (myRtcDataDate[1]-ASCII_NUMBER_MASK));
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x05, ((myRtcDataDate[2]-ASCII_NUMBER_MASK))<<4 | (myRtcDataDate[3]-ASCII_NUMBER_MASK));
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD,0x06, ((myRtcDataDate[4]-ASCII_NUMBER_MASK))<<4 | (myRtcDataDate[5]-ASCII_NUMBER_MASK));
				}
			}

		}
		vTaskDelay(1);
		taskYIELD();
	}
}

void hourFormat_task(void *pvParameters)
{
	static uint8_t received_data;
	UART_Type *currentUart;
	for(;;)
	{
		if(MENU_OP5 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP5 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTake(NewDataUart0, portMAX_DELAY))
			{
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				UART_WriteByte(currentUart, received_data);
				if(received_data == 0x31)
				{
					HOURS_REG_SIZE = 0x7F;
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x00);
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x02, HOURS_REG_SIZE);
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x80);
				}
				else if(received_data == 0x32)
				{
					HOURS_REG_SIZE = 0x3F;
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x00);
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x02, HOURS_REG_SIZE);
					delay(1000);
					I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x80);
				}
			}
			else if(xSemaphoreTake(NewDataUart3, portMAX_DELAY))
			{
				received_data = uart3Data;
				currentUart = DEMO_UART3;
				UART_WriteByte(currentUart, received_data);
				if(received_data == 0x32)
					if(received_data == 0x31)
					{
						HOURS_REG_SIZE = 0x7F;
						I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x00);
						delay(1000);
						I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x02, HOURS_REG_SIZE);
						delay(1000);
						I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x80);
					}
					else if(received_data == 0x32)
					{
						HOURS_REG_SIZE = 0x3F;
						I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x00);
						delay(1000);
						I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x02, HOURS_REG_SIZE);
						delay(1000);
						I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x80);
					}
			}
		}
		vTaskDelay(1);
		taskYIELD();
	}
}

void readHour_task(void *pvParameters)
{
	static uint8_t received_data;
	UART_Type *currentUart;
	static uint8_t uart0;
	static uint8_t uart3;
	uart0 = readHourUart0Flag;
	uart3 = readHourUart3Flag;
	for(;;)
	{
		if(MENU_OP6 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP6 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(uart0 == 1)
			{
				taskENTER_CRITICAL();
				readHourUart0Flag = 0;
				taskEXIT_CRITICAL();
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				if(ESCTERA != received_data)
				{
					UART_WriteByte(currentUart, asciiDate[5]);
					UART_WriteByte(currentUart, asciiDate[4]);
					UART_WriteBlocking(currentUart, pointsString, sizeof(pointsString) / sizeof(pointsString[0]));
					UART_WriteByte(currentUart, asciiDate[3]);
					UART_WriteByte(currentUart, asciiDate[2]);
					UART_WriteBlocking(currentUart, pointsString, sizeof(pointsString) / sizeof(pointsString[0]));
					UART_WriteByte(currentUart, asciiDate[1]);
					UART_WriteByte(currentUart, asciiDate[0]);
					UART_WriteBlocking(currentUart, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
					vTaskDelay(1000);
				}
			}
			if(uart3 == 1)
			{
				taskENTER_CRITICAL();
				readHourUart3Flag = 0;
				taskEXIT_CRITICAL();
				received_data = uart3Data;
				currentUart = DEMO_UART3;
				if(ESCTERA != received_data)
				{
					UART_WriteBlocking(currentUart, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
					delay(100);
					UART_WriteByte(currentUart, asciiDate[5]);
					delay(100);
					UART_WriteByte(currentUart, asciiDate[4]);
					delay(100);
					UART_WriteBlocking(currentUart, pointsString, sizeof(pointsString) / sizeof(pointsString[0]));
					delay(100);
					UART_WriteByte(currentUart, asciiDate[3]);
					delay(100);
					UART_WriteByte(currentUart, asciiDate[2]);
					delay(100);
					UART_WriteBlocking(currentUart, pointsString, sizeof(pointsString) / sizeof(pointsString[0]));
					delay(100);
					UART_WriteByte(currentUart, asciiDate[1]);
					delay(100);
					UART_WriteByte(currentUart, asciiDate[0]);
					delay(100);
					UART_WriteBlocking(currentUart, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
					vTaskDelay(1000);
				}
			}
		}
		vTaskDelay(100);
		taskYIELD();
	}
}

void readDate_task(void *pvParameters)
{
	static uint8_t received_data;
	UART_Type *currentUart;
	for(;;)
	{
		if(MENU_OP7 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP7 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(readDateUart0Flag  == 1)
			{
				received_data = uart0Data;
				readDateUart0Flag = 0;
				currentUart = DEMO_UART0;
				if(ESCTERA != received_data)
				{
					UART_WriteByte(currentUart, asciiDate[7]);
					UART_WriteByte(currentUart, asciiDate[6]);
					UART_WriteBlocking(currentUart, lineString, sizeof(lineString) / sizeof(lineString[0]));
					UART_WriteByte(currentUart, asciiDate[9]);
					UART_WriteByte(currentUart, asciiDate[8]);
					UART_WriteBlocking(currentUart, twentyString, sizeof(twentyString) / sizeof(twentyString[0]));
					UART_WriteByte(currentUart, asciiDate[11]);
					UART_WriteByte(currentUart, asciiDate[10]);
					UART_WriteBlocking(currentUart, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
					vTaskDelay(1000);
				}
			}
			else if(readDateUart3Flag == 1)
			{
				readDateUart3Flag = 0;
				currentUart = DEMO_UART3;
				if(ESCTERA != received_data)
				{
					UART_WriteBlocking(currentUart, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
					delay(100);
					UART_WriteByte(currentUart, asciiDate[7]);
					delay(100);
					UART_WriteByte(currentUart, asciiDate[6]);
					delay(100);
					UART_WriteBlocking(currentUart, lineString, sizeof(lineString) / sizeof(lineString[0]));
					delay(100);
					UART_WriteByte(currentUart, asciiDate[9]);
					delay(100);
					UART_WriteByte(currentUart, asciiDate[8]);
					delay(100);
					UART_WriteBlocking(currentUart, twentyString, sizeof(twentyString) / sizeof(twentyString[0]));
					delay(100);
					UART_WriteByte(currentUart, asciiDate[11]);
					delay(100);
					UART_WriteByte(currentUart, asciiDate[10]);
					delay(100);
					UART_WriteBlocking(currentUart, adjust4, sizeof(adjust4) / sizeof(adjust4[0]));
					vTaskDelay(1000);
				}
			}
		}
		vTaskDelay(1);
		taskYIELD();
	}
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
			if(xSemaphoreTake(NewDataUart0, portMAX_DELAY))
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
			if(xSemaphoreTake(NewDataUart3, portMAX_DELAY))
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

			if((ESCTERA == uart0Data) && (ESCTERA == uart3Data))
			{
				UART_WriteBlocking(DEMO_UART0, TerminalOneEndingString, sizeof(TerminalOneEndingString) / sizeof(TerminalOneEndingString[0]));
				UART_WriteBlocking(DEMO_UART0, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
				UART_WriteBlocking(DEMO_UART3, TerminalOneEndingString, sizeof(TerminalOneEndingString) / sizeof(TerminalOneEndingString[0]));
				UART_WriteBlocking(DEMO_UART3, NewLineString, sizeof(NewLineString) / sizeof(NewLineString[0]));
				delay(600000);
				vTaskDelete(chatHandle);
			}
		}
		vTaskDelay(100);
	}
}

void eco_task(void *pvParameters)
{
	static uint8_t key_pressed = FALSE;
	static uint8_t received_data;
	UART_Type *currentUart;
	vTaskDelete(lcdHandle);
	LCDNokia_clear();
	LCDNokia_clear();
	for(;;)
	{
		if (MENU_OP9 == xEventGroupGetBits(Event_uartHandle0) || MENU_OP9 == xEventGroupGetBits(Event_uartHandle3))
		{
			if(xSemaphoreTake(NewDataUart0, portMAX_DELAY))
			{
				received_data = uart0Data;
				currentUart = DEMO_UART0;
				if(ESCTERA != received_data)
				{
					UART_WriteByte(currentUart,received_data);
					LCDNokia_sendChar(received_data);
				}
			}
			else if(xSemaphoreTake(NewDataUart3, portMAX_DELAY))
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
		vTaskDelay(100);
		taskYIELD();
	}
}


static void printingReadMemMenu(UART_Type *base)
{
	xTaskCreate(readingI2C_task, "ReadingI2C_Task", 450, NULL, 2, &readingI2CHandle);//OPTION 1
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
	xTaskCreate(writingI2C_task, "WritingI2C_Task", 450, NULL, 2, &writingI2CHandle);//OPTION 2
	if(UART0 == base)
	{
		UART_WriteBlocking(DEMO_UART0, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART0, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART0, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART0, memoryWriteDirectionString, sizeof(memoryWriteDirectionString) / sizeof(memoryWriteDirectionString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
		UART_WriteBlocking(DEMO_UART0, lenghtToWriteString, sizeof(lenghtToWriteString) / sizeof(lenghtToWriteString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust4, sizeof(adjust4) / sizeof(adjust4[0]));
		UART_WriteBlocking(DEMO_UART0, savedTextString, sizeof(savedTextString) / sizeof(savedTextString[0]));
		UART_WriteBlocking(DEMO_UART0, adjust3, sizeof(adjust3) / sizeof(adjust3[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, memoryWriteDirectionString, sizeof(memoryWriteDirectionString) / sizeof(memoryWriteDirectionString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
		UART_WriteBlocking(DEMO_UART3, lenghtToWriteString, sizeof(lenghtToWriteString) / sizeof(lenghtToWriteString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust4, sizeof(adjust4) / sizeof(adjust4[0]));
		UART_WriteBlocking(DEMO_UART3, savedTextString, sizeof(savedTextString) / sizeof(savedTextString[0]));
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
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, setNewHourString, sizeof(setNewHourString) / sizeof(setNewHourString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
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
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, setNewDateString, sizeof(setNewDateString) / sizeof(setNewDateString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
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
		UART_WriteBlocking(DEMO_UART0, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
		UART_WriteBlocking(DEMO_UART0, setNewFormatOptionString, sizeof(setNewFormatOptionString) / sizeof(setNewFormatOptionString[0]));
	}
	else if(UART3 == base)
	{
		UART_WriteBlocking(DEMO_UART3, clearingCommand, sizeof(clearingCommand) / sizeof(clearingCommand[0]));
		UART_WriteBlocking(DEMO_UART3, blueScreeCommand, sizeof(blueScreeCommand) / sizeof(blueScreeCommand[0]));
		UART_WriteBlocking(DEMO_UART3, xandyPositioning, sizeof(xandyPositioning) / sizeof(xandyPositioning[0]));
		UART_WriteBlocking(DEMO_UART3, setNewFormatString, sizeof(setNewFormatString) / sizeof(setNewFormatString[0]));
		UART_WriteBlocking(DEMO_UART3, adjust2, sizeof(adjust2) / sizeof(adjust2[0]));
		UART_WriteBlocking(DEMO_UART3, setNewFormatOptionString, sizeof(setNewFormatOptionString) / sizeof(setNewFormatOptionString[0]));
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
	LCDNokia_clear();
	LCDNokia_gotoXY(0,0);
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


void getTime_task(void *pvParameters)
{
	/*Start Timer*/

	I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x80);

	for(;;)
	{
		if(FALSE == I2C_RtcRead(I2C0, RTC_DEVICE_ADD, 0x00, timeBuffer, 7))
		{
			UART_WriteBlocking(DEMO_UART0, errorString, sizeof(errorString) / sizeof(errorString[0]));
			delay(600000);
			UART_WriteBlocking(DEMO_UART0, deleteLine, sizeof(deleteLine) / sizeof(deleteLine [0]));
			UART_WriteBlocking(DEMO_UART0, adjut10, sizeof(adjut10) / sizeof(adjut10[0]));
		} //else I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x80);

		I2C_RtcRead(I2C0, RTC_DEVICE_ADD, 0x00, timeBuffer, 7);
		timeBuffer[0] = timeBuffer[0] & SECONDS_REG_SIZE;
		timeBuffer[1] = timeBuffer[1] & MINUTES_REG_SIZE;
		timeBuffer[2] = timeBuffer[2] & HOURS_REG_SIZE;
		timeBuffer[4] = timeBuffer[4] & DAY_REG_SIZE;
		timeBuffer[5] = timeBuffer[5] & MONTH_REG_SIZE;
		timeBuffer[6] = timeBuffer[6] & YEAR_REG_SIZE;

		asciiDate[0] = ((timeBuffer[0] & BCD_L)) + ASCII_NUMBER_MASK;
		asciiDate[1] = ((timeBuffer[0] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[2] = ((timeBuffer[1] & BCD_L)) + ASCII_NUMBER_MASK;
		asciiDate[3] = ((timeBuffer[1] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[4] = ((timeBuffer[2] & BCD_L)) + ASCII_NUMBER_MASK;
		asciiDate[5] = ((timeBuffer[2] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[6] = ((timeBuffer[4] & BCD_L))+ASCII_NUMBER_MASK;
		asciiDate[7] = ((timeBuffer[4] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[8] = ((timeBuffer[5] & BCD_L))+ASCII_NUMBER_MASK;
		asciiDate[9] = ((timeBuffer[5] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[10] = ((timeBuffer[6] & BCD_L))+ASCII_NUMBER_MASK;
		asciiDate[11] = ((timeBuffer[6] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		vTaskDelay(1000);
		taskYIELD();
	}
}

void serialTimeLCD( void *pvParameters)
{
	for(;;)
	{

		LCDNokia_clear();
		LCDNokia_gotoXY(25,0);
		LCDNokia_sendString("Hora:");
		LCDNokia_gotoXY(16,1);
		LCDNokia_sendChar(asciiDate[5]);
		LCDNokia_sendChar(asciiDate[4]);
		LCDNokia_sendChar(':');
		LCDNokia_sendChar(asciiDate[3]);
		LCDNokia_sendChar(asciiDate[2]);
		LCDNokia_sendChar(':');
		LCDNokia_sendChar(asciiDate[1]);
		LCDNokia_sendChar(asciiDate[0]);

		LCDNokia_gotoXY(22,3);
		LCDNokia_sendString("Fecha:");
		LCDNokia_gotoXY(8,4);
		LCDNokia_sendChar(asciiDate[7]);
		LCDNokia_sendChar(asciiDate[6]);
		LCDNokia_sendChar('-');
		LCDNokia_sendChar(asciiDate[9]);
		LCDNokia_sendChar(asciiDate[8]);
		LCDNokia_sendString("-20");
		LCDNokia_sendChar(asciiDate[11]);
		LCDNokia_sendChar(asciiDate[10]);

		vTaskDelay(100);

	}
}

uint16_t asciiToHex(uint8_t *string){
	while(*string){
		hexAddress = hexAddress << 4;
		if(*string >= 'A' && *string <= 'F'){
			hexAddress |= *string - ASCII_LETTER_MASK;
			*string++;
		}else{
			hexAddress |= *string - ASCII_NUMBER_MASK;
			*string++;
		}
	}
	return hexAddress;
}
