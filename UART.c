/*
 * UART.c
 *
 *  Created on: Feb 27, 2017
 *      Author: joab
 */

#include "UART.h"
#include "fsl_debug_console.h"


const char *to_send = "FreeRTOS UART driver example!\r\n";

uint8_t background_buffer[32];
uint8_t recv_buffer[1];

uart_rtos_handle_t handle;
struct _uart_handle t_handle;

uart_rtos_config_t uart_config =
{
		.base = UART0,
		.baudrate = UART_BAUDRATE,
		.parity = kUART_ParityDisabled,
		.stopbits = kUART_OneStopBit,
		.buffer = background_buffer,
		.buffer_size = sizeof(background_buffer),
};


void uart_init(void)
{
	uart_config.srcclk = CLOCK_GetFreq(UART0_CLK_SRC);
	UART_RTOS_Init(&handle, &t_handle, &uart_config);
}


uint8_t* uart_DataEcho(void)
{
	size_t n;

	/* Send data */
	UART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
	if (n > 0)        {
		/* Echo the received data */
		UART_RTOS_Send(&handle, (uint8_t *)recv_buffer, n);
		return (uint8_t*)recv_buffer;
	}
}



