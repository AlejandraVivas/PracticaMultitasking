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
#include "fsl_i2c_freertos.h"

/*#include "fsl_debug_console.h"*/
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "pin_mux.h"
#include "fsl_port.h"
#include "task.h"
#include "event_groups.h"

#include "fsl_dspi.h"


static i2c_rtos_handle_t i2c_handler;
port_pin_config_t pin;
#define Memory_Slave_Address (0x6F)
static i2c_master_transfer_t i2c_transfer;

static status_t SPI_init()void{

}

static status_t  Memory_init(void){

	i2c_master_config_t i2c_config;
	status_t i2c_status;
	uint32_t i2c_clk;
//	Pin B2 y Pin B1 used for I2C0
	pin.openDrainEnable = kPORT_OpenDrainEnable;
	pin.driveStrength = kPORT_LowDriveStrength;
	pin.lockRegister = kPORT_UnlockRegister;
	pin.mux = kPORT_MuxAlt2;
	pin.passiveFilterEnable = kPORT_PassiveFilterDisable;
	pin.pullSelect = kPORT_PullUp;
	pin.slewRate = kPORT_FastSlewRate;
	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinConfig(PORTC, 10, &pin); // SDA
	PORT_SetPinConfig(PORTC, 11, &pin); // SCL

	NVIC_SetPriority(I2C1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);

	/*I2C Config*/
	i2c_config.baudRate_Bps=100000;
	i2c_config.enableMaster=true;
	i2c_config.enableHighDrive=false;
	i2c_config.enableStopHold=false;
	i2c_config.glitchFilterWidth=0;
	i2c_clk = CLOCK_GetFreq(I2C1_CLK_SRC);
	i2c_status = I2C_RTOS_Init(&i2c_handler, I2C1, &i2c_config, i2c_clk);
}

void Memory_writeTask(void *pvParameters){

	status_t i2c_status;
	static uint8_t i2c_register;
	EventBits_t bits;
	Memory_init();
	i2c_register = 0x01;

	for(;;)
	{
		i2c_transfer.flags = kI2C_TransferDefaultFlag;
		i2c_transfer.slaveAddress = Memory_Slave_Address;
		i2c_transfer.direction = kI2C_Write;
		i2c_transfer.subaddress = 0xEEEE;
		i2c_transfer.subaddressSize = 2;
		i2c_transfer.data = &i2c_register;
		i2c_transfer.dataSize = 1;
		i2c_status = I2C_RTOS_Transfer(&i2c_handler, &i2c_transfer);
	 }
}

void Memory_readTask(void *pvParameters){

	EventBits_t bits;
	static uint8_t i2c_register1;
	status_t i2c_status;

		for(;;)
		{
				i2c_transfer.flags = kI2C_TransferDefaultFlag;
				i2c_transfer.slaveAddress = Memory_Slave_Address;
				i2c_transfer.direction = kI2C_Read;
				i2c_transfer.subaddress = 0xEEEE;
				i2c_transfer.subaddressSize = 2;
				i2c_transfer.data = &i2c_register1;
				i2c_transfer.dataSize = 1;
				i2c_status = I2C_RTOS_Transfer(&i2c_handler, &i2c_transfer);
				/* Read data periodically on this task */

				vTaskDelay(1000);
		}
}

#define Memory_setup_writetask_PRIORITY (4)
#define Memory_setup_readtask_PRIORITY (3)

int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  BOARD_BootClockRUN();


  /* Add your code here */

  /* Create RTOS task */
  xTaskCreate( Memory_readTask,"Memoryread", 4*configMINIMAL_STACK_SIZE, NULL, Memory_setup_readtask_PRIORITY, NULL);
  xTaskCreate( Memory_writeTask,"Memorywrite", 4*configMINIMAL_STACK_SIZE, NULL, Memory_setup_writetask_PRIORITY, NULL);


  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



