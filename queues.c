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
#include "queue.h"
#include "timers.h"
#include "queues.h"

QueueHandle_t I2C_queue_handler; // Declare the queue

I2C_stuctData_t I2C_TransferData;

void queue_receiver(void *pvParameters) {

	I2C_queue_handler = xQueueCreate(1, sizeof(I2C_TransferData)); // Creation of the queue

}
