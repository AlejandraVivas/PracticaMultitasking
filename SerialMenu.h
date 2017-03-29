/*
 * SerialMenu.h
 *
 *  Created on: 19/03/2017
 *      Author: Alejandra Meza
 */

#ifndef SOURCE_SERIALMENU_H_
#define SOURCE_SERIALMENU_H_

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "UART.h"
#include "GlobalFunctions.h"
#include "LCDNokia5110.h"

#define ESCTERA 0x1B
#define ESCBLUE 0x5D

#define UART0FLAG 1
#define UART3FLAG 2

#define OPTION_1 0x31
#define OPTION_2 0x32
#define OPTION_3 0x33
#define OPTION_4 0x34
#define OPTION_5 0x35
#define OPTION_6 0x36
#define OPTION_7 0x37
#define OPTION_8 0x38
#define OPTION_9 0x39

#define ENTERTERA 0xD
#define ENTERBLUE 0x5B
#define TERMINALONE 1
#define TERMINALTWO 2

void mainMenu0_task(void *pvParameters);
void mainMenu3_task(void *pvParameters);
void printingMenu(UART_Type *base);
void createSemaphoreMutex(void);

/*Uart 0*/
void readingI2C_task(void *pvParameters);
void writingI2C_task(void *pvParameters);
void setHour_task(void *pvParameters);
void setDate_task(void *pvParameters);
void hourFormat_task(void *pvParameters);
void readHour_task(void *pvParameters);
void readDate_task(void *pvParameters);
void chat_task(void *pvParameters);
void eco_task(void *pvParameters);







#endif /* SOURCE_SERIALMENU_H_ */
