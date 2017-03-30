/*
 * memHandler.c
 *
 *  Created on: 30/03/2017
 *      Author: Alejandra Meza
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "memHandler.h"


uint16_t hexAddress;
uint8_t counter = 0;

uint8_t memoryReadValue(uint8_t *address, uint8_t *lenght){
	uint16_t readAddress = asciiToHex(address);
	uint8_t readLenght = asciiToHex(lenght);

}

uint16_t asciiToHex(uint8_t *string){
		while(*string){
			hexAddress = hexAddress << 4;
			if(*string >= 'A' && *string <= 'F'){
				hexAddress |= *string - ASCII_LETTER_MASK;
				*string++;
			}else{
				hexAddress |= *string - ASCII_NUMBER_MASK;
				*string++;
			}
	}
		return hexAddress;
}

