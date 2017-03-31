/*
 * memHandler.h
 *
 *  Created on: 30/03/2017
 *      Author: Alejandra Meza
 */

#ifndef SOURCE_MEMHANDLER_H_
#define SOURCE_MEMHANDLER_H_

#define RTC_DEVICE_ADD 0x6F
#define MEM_DEVICE_ADD 0x50

#define ASCII_NUMBER_MASK 0x30	//Mask used to add or substract to an HEX number value to convert it from/to ASCII value
#define ASCII_LETTER_MASK 0x37	//Mask used to add or substract to an HEX letter value to convert it from/to ASCII value
#define BCD_H 0xF0				//Mask used to make a bitwise operation with the high part of the BCD data
#define BCD_L 0x0F				//Mask used to make a bitwise operation with the low part of the BCD data
#define SECONDS_REG_SIZE 0x7F	//Used to set reading boundaries according to the seconds register size
#define MINUTES_REG_SIZE 0x7F	//Used to set reading boundaries according to the minutes register size
#define HOURS_REG_SIZE 0x1F		//Used to set reading boundaries according to the hours register size
#define DAY_REG_SIZE 0x2F		//Used to set reading boundaries according to the days register size
#define MONTH_REG_SIZE 0x1F		//Used to set reading boundaries according to the month register size
#define YEAR_REG_SIZE 0xFF		//Used to set reading boundaries according to the year register size

#define ASCII_NUMBER_MASK 0x30	//Mask used to add or substract to an HEX number value to convert it from/to ASCII value
#define ASCII_LETTER_MASK 0x37	//Mask used to add or substract to an HEX letter value to convert it from/to ASCII value

uint16_t asciiToHex(uint8_t *string);

uint8_t memoryReadValue(uint8_t *address, uint8_t *lenght);


void getTime_task(void);
void sendTime(void);
void uartTest(void* param);

void createQueues(void);

static QueueHandle_t timeQueue;
static uint8_t fromQueue;
static SemaphoreHandle_t xMutex;

uint8_t* queueToUART();//


#endif /* SOURCE_MEMHANDLER_H_ */
