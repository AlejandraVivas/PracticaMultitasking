/*
 * SPI.c
 *
 *  Created on: 04/10/2016
 *      Author: Joab T
 */

#include "SPI.h"
#include "GPIO.h"

void SPI_init(const SPI_ConfigType* SPI_Config){
	SPI_clk(SPI_Config->SPI_Channel);
	GPIO_clockGating(SPI_Config->GPIOForSPI.GPIO_portName);
	GPIO_pinControlRegister(SPI_Config->GPIOForSPI.GPIO_portName,SPI_Config->GPIOForSPI.SPI_clk,&(SPI_Config->pinConttrolRegisterPORTD));
	GPIO_pinControlRegister(SPI_Config->GPIOForSPI.GPIO_portName,SPI_Config->GPIOForSPI.SPI_Sout,&(SPI_Config->pinConttrolRegisterPORTD));
	SPI_setMaster(SPI_Config->SPI_Channel,SPI_Config->SPI_Master);
	SPI_FIFO(SPI_Config->SPI_Channel,SPI_Config->SPI_EnableFIFO);
	SPI_enable(SPI_Config->SPI_Channel);
	SPI_ClockPolarity(SPI_Config->SPI_Channel,SPI_Config->SPI_Polarity);
	SPI_frameSize(SPI_Config->SPI_Channel,SPI_Config->frameSize);
	SPI_ClockPhase(SPI_Config->SPI_Channel,SPI_Config->SPI_Phase);
	SPI_baudRate(SPI_Config->SPI_Channel,SPI_Config->baudrate);
	SPI_mSBFirst(SPI_Config->SPI_Channel,SPI_MSB);
}

void SPI_sendOneByte(uint8 Data){
	SPI0_PUSHR = (Data);
	while(0 == (SPI0_SR & SPI_SR_TCF_MASK));
	SPI0_SR |= SPI_SR_TCF_MASK;
}

static void SPI_enable(SPI_ChannelType channel){
	switch(channel){
	case SPI_0:
		SPI0_MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	case SPI_1:
		SPI1_MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	case SPI_2:
		SPI2_MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	default:
		break;
	}
}

static void SPI_clk(SPI_ChannelType channel){
	switch(channel){
	case SPI_0:
		SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK;
		break;
	case SPI_1:
		SIM_SCGC6 |= SIM_SCGC6_SPI1_MASK;
		break;
	case SPI_2:
		SIM_SCGC3 |= SIM_SCGC3_SPI2_MASK;
		break;
	default:
		break;
	}

}

static void SPI_setMaster(SPI_ChannelType channel, SPI_MasterType masterOrSlave){
	switch(channel){
	case SPI_0:
		if(masterOrSlave)
			SPI0_MCR |= SPI_MCR_MSTR_MASK;
		else
			SPI0_MCR &= ~(SPI_MCR_MSTR_MASK);
		break;
	case SPI_1:
		if(masterOrSlave)
			SPI1_MCR |= SPI_MCR_MSTR_MASK;
		else
			SPI1_MCR &= ~(SPI_MCR_MSTR_MASK);
		break;
	case SPI_2:
		if(masterOrSlave)
			SPI2_MCR |= SPI_MCR_MSTR_MASK;
		else
			SPI2_MCR &= ~(SPI_MCR_MSTR_MASK);
		break;
	default:
		break;
	}
}

static void SPI_FIFO(SPI_ChannelType channel, SPI_EnableFIFOType enableOrDisable){
	switch(channel){
	case SPI_0:
		if(enableOrDisable)
			SPI0_MCR &= ~(SPI_MCR_DIS_RXF_MASK | SPI_MCR_DIS_TXF_MASK);
		else
			SPI0_MCR |= (SPI_MCR_DIS_RXF_MASK | SPI_MCR_DIS_TXF_MASK);
		break;
	case SPI_1:
		if(enableOrDisable)
			SPI1_MCR &= ~(SPI_MCR_DIS_RXF_MASK | SPI_MCR_DIS_TXF_MASK);
		else
			SPI1_MCR |= (SPI_MCR_DIS_RXF_MASK | SPI_MCR_DIS_TXF_MASK);
		break;
	case SPI_2:
		if(enableOrDisable)
			SPI2_MCR &= ~(SPI_MCR_DIS_RXF_MASK | SPI_MCR_DIS_TXF_MASK);
		else
			SPI2_MCR |= (SPI_MCR_DIS_RXF_MASK | SPI_MCR_DIS_TXF_MASK);
		break;
	default:
		break;
	}
}

static void SPI_ClockPolarity(SPI_ChannelType channel, SPI_PolarityType cpol){
	switch(channel){
	case SPI_0:
		if(cpol)
			SPI0_CTAR0 |= SPI_CTAR_CPOL_MASK;
		else
			SPI0_CTAR0 &= ~(SPI_CTAR_CPOL_MASK);
		break;
	case SPI_1:
		if(cpol)
			SPI1_CTAR0 |= SPI_CTAR_CPOL_MASK;
		else
			SPI1_CTAR0 &= ~(SPI_CTAR_CPOL_MASK);
		break;
	case SPI_2:
		if(cpol)
			SPI2_CTAR0 |= SPI_CTAR_CPOL_MASK;
		else
			SPI2_CTAR0 &= ~(SPI_CTAR_CPOL_MASK);
		break;
	default:
		break;
	}
}

static void SPI_ClockPhase(SPI_ChannelType channel, SPI_PhaseType cpha){
	switch(channel){
	case SPI_0:
		if(cpha)
			SPI0_CTAR0 |= SPI_CTAR_CPHA_MASK;
		else
			SPI0_CTAR0 &= ~(SPI_CTAR_CPHA_MASK);
		break;
	case SPI_1:
		if(cpha)
			SPI1_CTAR0 |= SPI_CTAR_CPHA_MASK;
		else
			SPI1_CTAR0 &= ~(SPI_CTAR_CPHA_MASK);
		break;
	case SPI_2:
		if(cpha)
			SPI2_CTAR0 |= SPI_CTAR_CPHA_MASK;
		else
			SPI2_CTAR0 &= ~(SPI_CTAR_CPHA_MASK);
		break;
	default:
		break;
	}
}

static void SPI_baudRate(SPI_ChannelType channel, uint32 baudRate){
	switch(channel){
	case SPI_0:
		SPI0_CTAR0 |= baudRate;
		break;
	case SPI_1:
		SPI1_CTAR0 |= baudRate;
		break;
	case SPI_2:
		SPI2_CTAR0 |= baudRate;
		break;
	default:
		break;
	}
}

static void SPI_mSBFirst(SPI_ChannelType channel, SPI_LSMorMSBType msb){
	switch(channel){
	case SPI_0:
		if(msb)
			SPI0_CTAR0 |= SPI_CTAR_LSBFE_MASK;
		else
			SPI0_CTAR0 &= ~(SPI_CTAR_LSBFE_MASK);
		break;
	case SPI_1:
		if(msb)
			SPI1_CTAR0 |= SPI_CTAR_LSBFE_MASK;
		else
			SPI1_CTAR0 &= ~(SPI_CTAR_LSBFE_MASK);
		break;
	case SPI_2:
		if(msb)
			SPI2_CTAR0 |= SPI_CTAR_LSBFE_MASK;
		else
			SPI2_CTAR0 &= ~(SPI_CTAR_LSBFE_MASK);
		break;
	default:
		break;
	}
}

static void SPI_frameSize(SPI_ChannelType channel, uint32 frameSize){
	switch(channel){
	case SPI_0:
		SPI0_CTAR0 &= ~(SPI_CTAR_FMSZ_MASK);
		SPI0_CTAR0 |= frameSize;
		break;
	case SPI_1:
		SPI1_CTAR0 &= ~(SPI_CTAR_FMSZ_MASK);
		SPI1_CTAR0 |= frameSize;
		break;
	case SPI_2:
		SPI2_CTAR0 &= ~(SPI_CTAR_FMSZ_MASK);
		SPI2_CTAR0 |= frameSize;
		break;
	default:
		break;
	}
}

void SPI_startTranference(SPI_ChannelType channel){
	switch(channel){
	case SPI_0:
		SPI0_MCR &= ~(SPI_MCR_HALT_MASK);
		break;
	case SPI_1:
		SPI1_MCR &= ~(SPI_MCR_HALT_MASK);
		break;
	case SPI_2:
		SPI2_MCR &= ~(SPI_MCR_HALT_MASK);
		break;
	default:
		break;
	}
}

void SPI_stopTranference(SPI_ChannelType channel){
	switch(channel){
	case SPI_0:
		SPI0_MCR |= SPI_MCR_HALT_MASK;
		break;
	case SPI_1:
		SPI1_MCR |= SPI_MCR_HALT_MASK;
		break;
	case SPI_2:
		SPI2_MCR |= SPI_MCR_HALT_MASK;
		break;
	default:
		break;
	}
}



