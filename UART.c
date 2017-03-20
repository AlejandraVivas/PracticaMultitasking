/*
 * UART.c
 *
 *  Created on: Feb 27, 2017
 *      Autor: AleVivas
 */

#include "UART.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_uart.h"

#include "pin_mux.h"
#include "clock_config.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/
uart_handle_t g_uart0Handle;
uart_handle_t g_uart3Handle;

uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
//extern void UART0_DriverIRQHandler(void);
//extern void UART3_DriverIRQHandler(void);

/*!
 * @brief Main function
 */
void uart0_init(void)
{
    uart_config_t config;
    uart_transfer_t xfer;
    uart_transfer_t sendXfer;
    uart_transfer_t receiveXfer;

    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = UART_BAUDRATE0;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(DEMO_UART0, &config, CLOCK_GetFreq(DEMO_UART0_CLKSRC));
}

void uart3_init(void)
{
    uart_config_t config;
    uart_transfer_t xfer;
    uart_transfer_t sendXfer;
    uart_transfer_t receiveXfer;

    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = UART_BAUDRATE3;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(DEMO_UART3, &config, CLOCK_GetFreq(DEMO_UART3_CLKSRC));
}
