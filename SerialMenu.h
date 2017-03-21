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

#define ESC 0x6D

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



void mainMenu_task(void *pvParameters);
void chat_task(void *pvParameters);
TaskHandle_t optionEightHandler();
void CreateChatTask(void);



#endif /* SOURCE_SERIALMENU_H_ */
