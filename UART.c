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

uint8_t background_buffer[32];
uint8_t background_buffer4[32];
uint8_t recv_bufferOne[1];
uint8_t recv_bufferFour[1];


uart_rtos_handle_t handle;
uart_rtos_handle_t handleFour;
struct _uart_handle t_handle;
struct _uart_handle t_handleFour;


//uint8_t clearingCommand[] = "\033[0;30;46m"; //clearing screen
//uint8_t blueScreeCommand[] = "\033[2J"; //Blue background and white letters
//uint8_t xandyPositioning[] = "\033[1;1H"; //x and y cursor position


uart_rtos_config_t uart0_config = {
		.base = UART0,
		.baudrate = UART_BAUDRATE0,
		.parity = kUART_ParityDisabled,
		.stopbits = kUART_OneStopBit,
		.buffer = background_buffer,
		.buffer_size = sizeof(background_buffer),
};

uart_rtos_config_t uart4_config = {
		.base = UART3,
		.baudrate = UART_BAUDRATE4,
		.parity = kUART_ParityDisabled,
		.stopbits = kUART_OneStopBit,
		.buffer = background_buffer4,
		.buffer_size = sizeof(background_buffer4),
};

void uart0_init(){
	uart0_config.srcclk = CLOCK_GetFreq(UART0_CLK_SRC);
	UART_RTOS_Init(&handle, &t_handle, &uart0_config);
	uart4_config.srcclk = CLOCK_GetFreq(UART3_CLK_SRC);
	UART_RTOS_Init(&handleFour, &t_handleFour, &uart4_config);
}


//void uartTerminalConfig(void)
//{
//	UART_RTOS_Send(&handle, clearingCommand, sizeof(clearingCommand));
//	UART_RTOS_Send(&handle, blueScreeCommand, sizeof(blueScreeCommand));
//	UART_RTOS_Send(&handle, xandyPositioning, sizeof(xandyPositioning));
//}

uint8_t UART_Echo(){
	int error;
	size_t n;
	/* Send data */
	UART_RTOS_Receive(&handle, recv_bufferOne, sizeof(recv_bufferOne), &n);
	if (n > 0)        {
		/* Echo the received data */
		UART_RTOS_Send(&handle, (uint8_t *)recv_bufferOne, n);
		return recv_bufferOne[0];
	}
}


uart_rtos_handle_t* teraHandler()
{
	return &handle;
}


uart_rtos_handle_t* blueHandler()
{
	return &handleFour;
}




