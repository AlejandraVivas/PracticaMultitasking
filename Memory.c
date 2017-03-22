/*
 * Memory.c
 *
 *  Created on: Mar 20, 2017
 *      Author: peter
 */

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

static i2c_rtos_handle_t i2c_handler;
port_pin_config_t pin;
#define Memory_Slave_Address (0x6F)
static i2c_master_transfer_t i2c_transfer;

/*status_t MEMORY_checkError(void){
	status_t i2c_status = kStatus_Success;
	static i2c_master_transfer_t i2c_transfer1;
	uint8_t status;

	if (kStatus_Success == i2c_status)
		{
			/* Read and check the FXOS8700CQ WHOAMI register */
		/*i2c_transfer1.flags = kI2C_TransferDefaultFlag;
		i2c_transfer1.slaveAddress = 0x50;
		i2c_transfer1.direction = kI2C_Read;
		i2c_transfer1.subaddress = 0xEEEE;
		i2c_transfer1.subaddressSize = 2;
		i2c_transfer1.data = &status;
		i2c_transfer1.dataSize = 2;
			i2c_status = I2C_RTOS_Transfer(&i2c_handler, &i2c_transfer1);
		}
	if(status == 9){
		i2c_status = kStatus_Success;
	}
	else{
		i2c_status = kStatus_Fail;
	}

	return i2c_status;

}*/

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

	/*SET I2C master Configuration */
	i2c_config.baudRate_Bps=100000;
	i2c_config.enableMaster=true;
	i2c_config.enableHighDrive=false;
	i2c_config.enableStopHold=false;
	i2c_config.glitchFilterWidth=0;
	/*Get bus clock frecuency (I2C FRECUENCY)*/
	i2c_clk = CLOCK_GetFreq(I2C1_CLK_SRC);

	/*iNITITALIZE i2c */
	i2c_status = I2C_RTOS_Init(&i2c_handler, I2C1, &i2c_config, i2c_clk);
}

void Memory_writeTask(void *pvParameters){

	status_t i2c_status;
	static uint8_t i2c_register;
	Memory_init();
	i2c_register = 0x01;

	for(;;)
	{
		i2c_transfer.flags = kI2C_TransferDefaultFlag; /* A transfer flag which controls the transfer. */
		i2c_transfer.slaveAddress = Memory_Slave_Address; /* 7-bit slave address. */
		i2c_transfer.direction = kI2C_Write; /* A transfer direction, read or write. */
		i2c_transfer.subaddress = 0xEEEE; /* A sub address. Transferred MSB first. */
		i2c_transfer.subaddressSize = 2; /* A size of the command buffer. */
		i2c_transfer.data = &i2c_register; /* A transfer buffer. */
		i2c_transfer.dataSize = 1; /* A transfer size. */

		i2c_status = I2C_RTOS_Transfer(&i2c_handler, &i2c_transfer); // i2c WRITE STATUS
		// readen data will be in i2c_transfer slots
	 }
}

void Memory_readTask(void *pvParameters){

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

