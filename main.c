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

#include <string.h>

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "DataTypeDefinitions.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* SDK drivers*/
#include "fsl_port.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"



/* Task priorities. */
#define Buttons_task_PRIORITY (configMAX_PRIORITIES - 9)

/*Buttons value in port c*/
#define buttonOne 0x20 /*Bit 5, port C 0010_0000*/
#define buttonTwo 0x80 /*Bit 7, port C, 1000_0000*/
#define buttonThree 0x0 /*Bit 0, port C, 0000_0000*/
#define buttonFour 0x200 /*Bit 9, port C, 0010_0000_0000*/
#define buttonFive 0x100 /*Bit 8, port C, 0001_0000_0000*/
#define buttonSix 0x8 /*Bit 3, port C, 0000_1000*/

/*Flags to check if a button was pressed*/
uint8_t buttonOneFlag = FALSE;
uint8_t buttonTwoFlag = FALSE;
uint8_t buttonThreeFlag = FALSE;
uint8_t buttonFourFlag = FALSE;
uint8_t buttonFiveFlag = FALSE;
uint8_t buttonSixFlag = FALSE;


/*Port c interrupt handler function*/
void PORTC_IRQHandler(void)
{
	uint32_t bitNumber = 0;
	bitNumber = GPIO_GetPinsInterruptFlags(GPIOC);
	switch(bitNumber)
	{
	case buttonOne:
		GPIO_ClearPinsInterruptFlags(GPIOC, buttonOne);
		buttonOneFlag = TRUE;
	case buttonTwo:
		GPIO_ClearPinsInterruptFlags(GPIOC, buttonTwo);
		buttonTwoFlag = TRUE;
	case buttonThree:
		GPIO_ClearPinsInterruptFlags(GPIOC, buttonThree);
		buttonThreeFlag = TRUE;
	case buttonFour:
		GPIO_ClearPinsInterruptFlags(GPIOC, buttonFour);
		buttonFourFlag = TRUE;
	case buttonFive:
		GPIO_ClearPinsInterruptFlags(GPIOC, buttonFive);
		buttonFiveFlag = TRUE;
	case buttonSix:
		GPIO_ClearPinsInterruptFlags(GPIOC, buttonSix);
		buttonSixFlag = TRUE;
	}

}
void pressedButtons(void *pvParameters);


/*!
 * @brief Application entry point.
 */
int main(void) {
	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	/* Add your code here */

	/* Create RTOS task */
	xTaskCreate(pressedButtons, "Pressed_Buttons", configMINIMAL_STACK_SIZE, NULL, Buttons_task_PRIORITY, NULL);
	vTaskStartScheduler();

	for(;;) { /* Infinite loop to avoid leaving the main function */
		__asm("NOP"); /* something to use as a breakpoint stop while looping */
	}
}

void pressedButtons(void *pvParameters)
{
	for (;;)
	{
		if(TRUE == buttonOneFlag)
		{

		}
		else if(TRUE == buttonTwoFlag)
		{

		}
		else if(TRUE == buttonThreeFlag)
		{

		}
		else if(TRUE == buttonFourFlag)
		{

		}
		else if(TRUE == buttonFiveFlag)
		{

		}
		else if(TRUE == buttonSixFlag)
		{

		}
	}
}

