/*
 * memHandler.h
 *
 *  Created on: 30/03/2017
 *      Author: Alejandra Meza
 */

#ifndef SOURCE_MEMHANDLER_H_
#define SOURCE_MEMHANDLER_H_


#define ASCII_NUMBER_MASK 0x30	//Mask used to add or substract to an HEX number value to convert it from/to ASCII value
#define ASCII_LETTER_MASK 0x37	//Mask used to add or substract to an HEX letter value to convert it from/to ASCII value

uint16_t asciiToHex(uint8_t *string);

uint8_t memoryReadValue(uint8_t *address, uint8_t *lenght);

#endif /* SOURCE_MEMHANDLER_H_ */
