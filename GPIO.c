/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	7/09/2014
	\todo
	    Interrupts are not implemented in this API implementation.
 */
#include "MK64F12.h"
#include "GPIO.h"

uint8 FlagPortA = FALSE;
uint8 FlagPortB = FALSE;
uint8 FlagPortC = FALSE;
uint8 FlagPortD = FALSE;
uint8 FlagPortE = FALSE;

void PORTA_IRQHandler(){
	FlagPortA = TRUE;
	GPIO_clearInterrupt(GPIOA);
}

void PORTB_IRQHandler(){
	FlagPortB = TRUE;
	GPIO_clearInterrupt(GPIOB);
}

void PORTC_IRQHandler(){
	FlagPortC = TRUE;
	GPIO_clearInterrupt(GPIOC);
}

void PORTD_IRQHandler(){
	FlagPortD = TRUE;
	GPIO_clearInterrupt(GPIOD);
}

void PORTE_IRQHandler(){
	FlagPortE = TRUE;
	GPIO_clearInterrupt(GPIOE);
}

void GPIO_clearInterrupt(GPIO_portNameType portName){
	switch(portName)/** Selecting the GPIO for clock enabling*/
	{
		case GPIOA: /** GPIO A is selected*/
			PORTA_ISFR=0xFFFFFFFF;
			break;
		case GPIOB: /** GPIO B is selected*/
			PORTB_ISFR=0xFFFFFFFF;
			break;
		case GPIOC: /** GPIO C is selected*/
			PORTC_ISFR = 0xFFFFFFFF;
			break;
		case GPIOD: /** GPIO D is selected*/
			PORTD_ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE_ISFR=0xFFFFFFFF;
			break;
	}// end switch
}

uint8 GPIO_clockGating(GPIO_portNameType portName)
{
	/*!
	 * \dot
	digraph G {
	main -> parse -> execute;
	main -> init;
	main -> cleanup;
	execute -> make_string;
	execute -> printf
	init -> make_string;
	main -> printf;
	execute -> compare;
	 }
	 \enddot
	*/
	switch(portName)/** Selecting the GPIO for clock enabling*/
			{
				case GPIOA: /** GPIO A is selected*/
					SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
					break;
				case GPIOB: /** GPIO B is selected*/
					SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
					break;
				case GPIOC: /** GPIO C is selected*/
					SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
					break;
				case GPIOD: /** GPIO D is selected*/
					SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
					break;
				case GPIOE: /** GPIO E is selected*/
					SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
					break;
				default: /**If doesn't exist the option*/
					return(FALSE);
			}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

uint8 GPIO_pinControlRegister(GPIO_portNameType portName,uint8 pin,const GPIO_pinControlRegisterType*  pinControlRegister)
{

	switch(portName)
		{
		case GPIOA:/** GPIO A is selected*/
			PORTA_PCR(pin)= *pinControlRegister;
			break;
		case GPIOB:/** GPIO B is selected*/
			PORTB_PCR(pin)= *pinControlRegister;
			break;
		case GPIOC:/** GPIO C is selected*/
			PORTC_PCR(pin)= *pinControlRegister;
			break;
		case GPIOD:/** GPIO D is selected*/
			PORTD_PCR(pin)= *pinControlRegister;
			break;
		case GPIOE: /** GPIO E is selected*/
			PORTE_PCR(pin)= *pinControlRegister;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
		}
	/**Successful configuration*/
	return(TRUE);
}

void GPIO_dataDirectionPIN(GPIO_portNameType portName, uint8 State, uint8 pin){

	switch(portName){
		case GPIOA:/** GPIO A is selected*/
			if(State == TRUE)
				GPIOA_PDDR |= 1 << pin;
			else
				GPIOA_PDDR |= 0 << pin;
			break;
		case GPIOB:/** GPIO B is selected*/
			if(State == TRUE)
				GPIOB_PDDR |= 1 << pin;
			else
				GPIOB_PDDR |= 0 << pin;
			break;
		case GPIOC:/** GPIO C is selected*/
			if(State == TRUE)
				GPIOC_PDDR |= 1 << pin;
			else
				GPIOC_PDDR |= 0 << pin;
			break;
		case GPIOD:/** GPIO D is selected*/
			if(State == TRUE)
				GPIOD_PDDR |= 1 << pin;
			else
				GPIOD_PDDR |= 1 << pin;
			break;
		case GPIOE:/** GPIO E is selected*/
			if(State == TRUE)
				GPIOE_PDDR |= 1 << pin;
			else
				GPIOE_PDDR |= 1 << pin;
			break;
		default:
			break;
	}
}

void GPIO_tooglePIN(GPIO_portNameType portName, uint8 pin){

	switch(portName){
			case GPIOA:/** GPIO A is selected*/
				GPIOA_PTOR |= 1 << pin;
				break;
			case GPIOB:/** GPIO B is selected*/
				GPIOB_PTOR |= 1 << pin;
				break;
			case GPIOC:/** GPIO C is selected*/
				GPIOC_PTOR |= 1 << pin;
				break;
			case GPIOD:/** GPIO D is selected*/
				GPIOD_PTOR |= 1 << pin;
				break;
			case GPIOE:/** GPIO E is selected*/
				GPIOE_PTOR |= 1 << pin;
				break;
			default:
				break;
		}
}

void GPIO_setPIN(GPIO_portNameType portName, uint8 pin){
	switch(portName){
		case GPIOA:
			GPIOA_PSOR = 1 << pin;
			break;
		case GPIOB:
			GPIOB_PSOR = 1 << pin;
			break;
		case GPIOC:
			GPIOC_PSOR = 1 << pin;
			break;
		case GPIOD:
			GPIOD_PSOR = 1 << pin;
			break;
		case GPIOE:
			GPIOE_PSOR = 1 << pin;
			break;
	}
}

void GPIO_writePORT(GPIO_portNameType portName, uint32 Data ){
	switch(portName){
			case GPIOA:
				GPIOA_PDOR = Data;
				break;
			case GPIOB:
				GPIOB_PDOR = Data;
				break;
			case GPIOC:
				GPIOC_PDOR = Data;
				break;
			case GPIOD:
				GPIOD_PDOR = Data;
				break;
			case GPIOE:
				GPIOE_PDOR = Data;
				break;
			default:
				break;
	}
}

uint32 GPIO_readPORT(GPIO_portNameType portName){
	uint32 portValue;
	switch(portName){
		case GPIOA: /** GPIO A is selected*/
			portValue = GPIOA_PDIR;
			break;
		case GPIOB: /** GPIO B is selected*/
			portValue = GPIOB_PDIR;
			break;
		case GPIOC: /** GPIO C is selected*/
			portValue = GPIOC_PDIR;
			break;
		case GPIOD: /** GPIO D is selected*/
			portValue = GPIOD_PDIR;
			break;
		case GPIOE: /** GPIO E is selected*/
			portValue = GPIOE_PDIR;
			break;
		default:
			portValue = FALSE;
			break;
	}
	return(portValue);
}

uint8 GPIO_readPIN(GPIO_portNameType portName, uint8 pin){
	uint8 pinValue;
	switch(portName){
		case GPIOA: /** GPIO A is selected*/
			pinValue = (GPIOA_PDIR & 1 << pin) >> pin;
			break;
		case GPIOB: /** GPIO B is selected*/
			pinValue = (GPIOB_PDIR & 1 << pin) >> pin;
			break;
		case GPIOC: /** GPIO C is selected*/
			pinValue = (GPIOC_PDIR & 1 << pin) >> pin;
			break;
		case GPIOD: /** GPIO D is selected*/
			pinValue = (GPIOD_PDIR & 1 << pin) >> pin;
			break;
		case GPIOE: /** GPIO E is selected*/
			pinValue = (GPIOE_PDIR & 1 << pin) >> pin;
			break;
		default:
			return(FALSE);
			break;
	}
	return(pinValue);
}

void GPIO_clearPIN(GPIO_portNameType portName, uint8 pin){
	switch(portName){
		case GPIOA:
			GPIOA_PCOR = 1 << pin;
			break;
		case GPIOB:
			GPIOB_PCOR = 1 << pin;
			break;
		case GPIOC:
			GPIOC_PCOR = 1 << pin;
			break;
		case GPIOD:
			GPIOD_PCOR = 1 << pin;
			break;
		case GPIOE:
			GPIOE_PCOR = 1 << pin;
			break;
		default:
			break;
		}
}

void GPIO_dataDirectionPORT(GPIO_portNameType portName ,uint32 direction){
	switch(portName){
		case GPIOA:/** GPIO A is selected*/
			GPIOA_PDDR = ~(direction);
			break;
		case GPIOB:/** GPIO B is selected*/
			GPIOB_PDDR = ~(direction);
			break;
		case GPIOC:/** GPIO C is selected*/
			GPIOC_PDDR = ~(direction);
			break;
		case GPIOD:/** GPIO D is selected*/
			GPIOD_PDDR = ~(direction);
			break;
		case GPIOE:/** GPIO E is selected*/
			GPIOE_PDDR = ~(direction);
			break;
		default:
			break;
	}
}


