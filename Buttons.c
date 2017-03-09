/*
 * Buttons.c
 *
 *  Created on: 08/03/2017
 *      Author: Alejandra Meza
 */

#include "Buttons.h"
#include "DataTypeDefinitions.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"


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
	case BUTTON_ONE:
		GPIO_ClearPinsInterruptFlags(GPIOC, BUTTON_ONE);
		buttonOneFlag = TRUE;
	case BUTTON_TWO:
		GPIO_ClearPinsInterruptFlags(GPIOC, BUTTON_TWO);
		buttonTwoFlag = TRUE;
	case BUTTON_THREE:
		GPIO_ClearPinsInterruptFlags(GPIOC, BUTTON_THREE);
		buttonThreeFlag = TRUE;
	case BUTTON_FOUR:
		GPIO_ClearPinsInterruptFlags(GPIOC, BUTTON_FOUR);
		buttonFourFlag = TRUE;
	case BUTTON_FIVE:
		GPIO_ClearPinsInterruptFlags(GPIOC, BUTTON_FIVE);
		buttonFiveFlag = TRUE;
	case BUTTON_SIX:
		GPIO_ClearPinsInterruptFlags(GPIOC, BUTTON_SIX);
		buttonSixFlag = TRUE;
	}

}


void pressedButtons_task(void *pvParameters)
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


