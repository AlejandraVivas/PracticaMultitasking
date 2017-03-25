/*
 * queues.c
 *
 *  Created on: Mar 22, 2017
 *      Author: peter
 */

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "stdio.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "queue.h"

#include "task.h"
#include "timers.h"
#include "queues.h"
#include "DataTypeDefinitions.h"

QueueHandle_t I2C_queue_handler; // Declare the queue

I2C_stuctData_t I2C_TransferData;


/*Function in charge of reading the I2C queue*/
void I2Cqueue_receiver(void *pvParameters) {

	I2C_stuctData_t *NewI2C_data_pointer = 0x00;
	UBaseType_t uxNumOfItems;

	I2C_queue_handler = xQueueCreate(1 , sizeof( I2C_TransferData) ); // Creation of the queue

	uxNumOfItems = uxQueueMessagesWaiting (I2C_queue_handler);

	if (uxNumOfItems != FALSE)
	{
		xQueueReceive(I2C_queue_handler, &NewI2C_data_pointer, NULL);
	}
}

void TeraTermqueue_reciever(void *pvParameters) {

}

void queue_reciever(void *pvParameters) {

}


//uxQueueMessagesWaiting()


//xQueueSend(I2C_queue_handler, I2C_pointer, NULL);
//xQueueReceive()
