/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/


/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "string.h"


#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "DataTypeDefinitions.h"
#include "UART.h"
#include "GlobalFunctions.h"
#include "SerialMenu.h"
#include "SPI.h"
#include "LCDNokia5110.h"
#include "I2C.h"


/* SDK drivers*/
#include "fsl_port.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"


/* Task priorities. */
TaskHandle_t lcdHandle;

//Todo lo coppiado y pegado en Buttons.c


/*!
 * @brief Application entry poin t.
 */
int main(void) {
	/* Init board hardware. */


	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	BOARD_I2C_ConfigurePins();
	NVIC_SetPriority(DEMO_UART0_RX_TX_IRQn, 5);
	NVIC_SetPriority(DEMO_UART3_RX_TX_IRQn, 5);
	NVIC_SetPriority(SPI0_IRQn, 5);

	gpio_pin_config_t config_gpio = { kGPIO_DigitalInput, 0 };
	port_pin_config_t config =
	{
			kPORT_PullDisable,
			kPORT_FastSlewRate,
			kPORT_PassiveFilterDisable,
			kPORT_OpenDrainDisable,
			kPORT_LowDriveStrength,
			kPORT_MuxAsGpio,
			kPORT_UnlockRegister
	};


	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortE);;

 	PORT_SetPinConfig(PORTC, BIT5, &config);
	PORT_SetPinConfig(PORTC, BIT7, &config);
	PORT_SetPinConfig(PORTC, BIT0, &config);
	PORT_SetPinConfig(PORTC, BIT9, &config);

	GPIO_PinInit(GPIOC, BIT5, &config_gpio);
	GPIO_PinInit(GPIOC, BIT7, &config_gpio);
	GPIO_PinInit(GPIOC, BIT0, &config_gpio);
	GPIO_PinInit(GPIOC, BIT9, &config_gpio);

	PORT_SetPinInterruptConfig(PORTC, BIT5, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTC, BIT7, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTC, BIT0, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTC, BIT9, kPORT_InterruptFallingEdge);
	/* Enable the interrupt. */

	NVIC_SetPriority( PORTC_IRQn, 1);
	NVIC_EnableIRQ( PORTC_IRQn);

	i2c_init();

	spi_init();

	LCDNokia_init();
	LCDNokia_clear();

	createSemaphoreMutex();


	uart_init();



	 printingMenu(DEMO_UART0);
	 printingMenu(DEMO_UART3);

	/* Add your code here */

	/* Create RTOS task */
	xTaskCreate(mainMenu0_task, "MainMenu0_Task", configMINIMAL_STACK_SIZE, NULL,5, NULL);
	xTaskCreate(mainMenu3_task, "MainMenu0_Task", configMINIMAL_STACK_SIZE, NULL,5, NULL);
	xTaskCreate(getTime_task, "GetTime_Task", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(serialTimeLCD, "serialTimeLCD", configMINIMAL_STACK_SIZE, NULL, 3,&lcdHandle);

	vTaskStartScheduler();

	for(;;) { /* Infinite loop to avoid leaving the main function */
		__asm("NOP"); /* something to use as a breakpoint stop while looping */
	}
}


