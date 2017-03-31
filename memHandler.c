/*
 * memHandler.c
 *
 *  Created on: 30/03/2017
 *      Author: Alejandra Meza
 */

#include "FreeRTOS.h"
#include "I2C.h"
#include "queue.h"
#include "fsl_debug_console.h"
#include "semphr.h"
#include "task.h"
#include "UART.h"

#include "memHandler.h"

uint16_t hexAddress;
uint8_t counter = 0;

uint8_t memoryReadValue(uint8_t *address, uint8_t *lenght){
	uint16_t readAddress = asciiToHex(address);
	uint8_t readLenght = asciiToHex(lenght);

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

void createQueues(void)
{
	timeQueue = xQueueCreate(12,sizeof(uint8_t));
}

void sendTime(void)
{
	uint8_t fromQueue[12];
	uint8_t qCounter = 0;
	uint8_t *ptrFromQueue = fromQueue;

	for(;;)
	{
		for(qCounter;qCounter<=11;qCounter++)
		{
			xQueueReceive(timeQueue, &fromQueue[qCounter], portMAX_DELAY);
		}
		qCounter = 0;
	}

	PRINTF("Time: %c", fromQueue[0]);
}
void getTime_task(void)
{
	uint8_t timeBuffer[7];
	uint8_t qCounter=0;
	uint8_t asciiDate[12];
	uint8_t *ptrToDate = asciiDate;

	/*Start Timer*/
	I2C_RtcWrite(I2C0, RTC_DEVICE_ADD, 0x00, 0x80);

	while(1){
		//xSemaphoreTake(xMutex,portMAX_DELAY);
		I2C_RtcRead(I2C0, RTC_DEVICE_ADD, 0x00, timeBuffer, 7);
		//xSemaphoreGive(xMutex);

		timeBuffer[0] = timeBuffer[0] & SECONDS_REG_SIZE;
		timeBuffer[1] = timeBuffer[1] & MINUTES_REG_SIZE;
		timeBuffer[2] = timeBuffer[2] & HOURS_REG_SIZE;
		timeBuffer[4] = timeBuffer[4] & DAY_REG_SIZE;
		timeBuffer[5] = timeBuffer[5] & MONTH_REG_SIZE;
		timeBuffer[6] = timeBuffer[6] & YEAR_REG_SIZE;

		asciiDate[0] = ((timeBuffer[0] & BCD_L)) + ASCII_NUMBER_MASK;
		asciiDate[1] = ((timeBuffer[0] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[2] = ((timeBuffer[1] & BCD_L))+ASCII_NUMBER_MASK;
		asciiDate[3] = ((timeBuffer[1] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[4] = ((timeBuffer[2] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[5] = ((timeBuffer[2] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[6] = ((timeBuffer[4] & BCD_L))+ASCII_NUMBER_MASK;
		asciiDate[7] = ((timeBuffer[4] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[8] = ((timeBuffer[5] & BCD_L))+ASCII_NUMBER_MASK;
		asciiDate[9] = ((timeBuffer[5] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[10] = ((timeBuffer[6] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[11] = ((timeBuffer[6] & BCD_H)>>4)+ASCII_NUMBER_MASK;

//		for(qCounter;qCounter<=11;qCounter++){
//			xQueueSend(timeQueue,&asciiDate[qCounter],1000);
//		}
//		qCounter=0;

		PRINTF("Time: %c%c:%c%c:%c%c  Date: %c%c/%c%c/%c%c\r",  (char)asciiDate[5],
																(char)asciiDate[4],
																(char)asciiDate[3],
																(char)asciiDate[2],
																(char)asciiDate[1],
																(char)asciiDate[0],
																(char)asciiDate[11],
																(char)asciiDate[10],
																(char)asciiDate[9],
																(char)asciiDate[8],
																(char)asciiDate[7],
																(char)asciiDate[6]);

		vTaskDelay(100);
	}
}

