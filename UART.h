/*
 * UART.h
 *
 *  Created on: Feb 27, 2017
 *      Author: joab
 */

#ifndef SOURCE_UART_H_
#define SOURCE_UART_H_

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define UART_BAUDRATE0 115200U
#define UART_BAUDRATE4 9600U
#define ECHO_BUFFER_LENGTH 1
#define DEMO_UART0 UART0
#define DEMO_UART4 UART4
#define DEMO_UART0_CLKSRC UART0_CLK_SRC
#define DEMO_UART4_CLKSRC UART4_CLK_SRC
#define DEMO_UART0_RX_TX_IRQn UART0_RX_TX_IRQn
#define DEMO_UART4_RX_TX_IRQn UART3_RX_TX_IRQn

typedef enum {UART_0,UART_1,UART_2,UART_3,UART_4,UART_5}UART_ChannelType;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//void uartInit_task(void *parameter);
void UART_UserCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);
void uart0_init();
uint8_t UART_Echo();
//void uartTerminalConfig(void);
uart_rtos_handle_t* teraHandler();
uart_rtos_handle_t* blueHandler();


#endif /* SOURCE_UART_H_ */
