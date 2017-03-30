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
#include "memHandler.h"

#define ESCTERA 0x1B

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


/**Events for menu options*/
#define MENU_OP1 (1 << 0)
#define MENU_OP2 (1 << 1)
#define MENU_OP3 (1 << 2)
#define MENU_OP4 (1 << 3)
#define MENU_OP5 (1 << 4)
#define MENU_OP6 (1 << 5)
#define MENU_OP7 (1 << 6)
#define MENU_OP8 (1 << 7)
#define MENU_OP9 (1 << 8) //SPI

#define ENTERTERA 0xD
#define ENTERBLUE 0x5B
#define ESCTERA 0x1B
#define ESCBLUE 0x5D


void mainMenu0_task(void *pvParameters);
void mainMenu3_task(void *pvParameters);
void printingMenu(UART_Type *base);
void createSemaphoreMutex(void);
uint8_t getMemoryAddress(uint8_t address_digit, uint8_t Address_counter);

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



static void printingReadMemMenu(UART_Type *base);
static void printingWriteMemMenu(UART_Type *base);
static void printingSetHourMenu(UART_Type *base);
static void printingSetDateMenu(UART_Type *base);
static void printingSetFormatMenu(UART_Type *base);
static void printingReadHourMenu(UART_Type *base);
static void printingReadDateMenu(UART_Type *base);
static void printingChatMenu(UART_Type *base);
static void printingEcoMenu(UART_Type *base);



#endif /* SOURCE_SERIALMENU_H_ */
