/*
 * UART.c
 *
 *  Created on: Feb 27, 2017
 *      Author: joab
 */

#include "UART.h"
#include "fsl_debug_console.h"


uint8_t background_buffer[32];
uint8_t recv_buffer[1];

const char *to_send1 = "\033[0;30;46m";
const char *to_send2 = "\033[2J";
const char *to_send3 = "\033[1;1H";

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

void uartInit_task(void *pvParameters)
{
    int error;
    size_t n;

    uart_config.srcclk = CLOCK_GetFreq(DEMO_UART_CLKSRC);
    uart_config.base = DEMO_UART;

    if (0 > UART_RTOS_Init(&handle, &t_handle, &uart_config))
    {
        vTaskSuspend(NULL);
    }

    /* Send some data */
    if (0 > UART_RTOS_Send(&handle, (uint8_t *)to_send1, strlen(to_send1)))
    {
        vTaskSuspend(NULL);
    }
    if (0 > UART_RTOS_Send(&handle, (uint8_t *)to_send2, strlen(to_send2)))
    {
        vTaskSuspend(NULL);
    }
    if (0 > UART_RTOS_Send(&handle, (uint8_t *)to_send3, strlen(to_send3)))
    {
        vTaskSuspend(NULL);
    }

    /* Send data */
    do
    {
        error = UART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
        if (n > 0)
        {
            /* send back the received data */
            UART_RTOS_Send(&handle, (uint8_t *)recv_buffer, n);
        }
       // vTaskDelay(1000);
    } while (kStatus_Success == error);

    UART_RTOS_Deinit(&handle);

    vTaskSuspend(NULL);
}


