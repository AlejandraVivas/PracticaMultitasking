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
#define UART_BAUDRATE 115200U

#define DEMO_UART UART0
#define DEMO_UART_CLKSRC UART0_CLK_SRC
#define DEMO_UART_RX_TX_IRQn UART0_RX_TX_IRQn

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
uint8_t* uart_DataEcho(void);
void uart_init(void);


#endif /* SOURCE_UART_H_ */
