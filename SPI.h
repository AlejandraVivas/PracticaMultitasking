/**
	\file 
	\brief 
		This is a starter file to use the Nokia 5510 LCD. 
		The LCD is connected as follows:
		reset-PDT0
		CE-GND
		DC-PTD3
		DIN-PTD2
		CLK-PTD1
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	1/08/2015
	\todo
	    The SPI device driver needs to be completed.
 */

#ifndef SPI_H_
#define SPI_H_
#include <MK64F12.h>
#include "DataTypeDefinitions.h"
#include "GPIO.h"


/** Constant that represent the clock enable for the SPI modules */
#define SPI0_CLOCK_GATING 0x1000
#define SPI1_CLOCK_GATING 0x2000
#define SPI2_CLOCK_GATING 0x1000

/** Constant to define the frame size in the SPI transmission */
#define SPI_FSIZE_4 SPI_CTAR_FMSZ(3)
#define SPI_FSIZE_5 SPI_CTAR_FMSZ(4)
#define SPI_FSIZE_6 SPI_CTAR_FMSZ(5)
#define SPI_FSIZE_7 SPI_CTAR_FMSZ(6)
#define SPI_FSIZE_8 SPI_CTAR_FMSZ(7)
#define SPI_FSIZE_9 SPI_CTAR_FMSZ(8)
#define SPI_FSIZE_10 SPI_CTAR_FMSZ(9)
#define SPI_FSIZE_11 SPI_CTAR_FMSZ(10) 
#define SPI_FSIZE_12 SPI_CTAR_FMSZ(11)
#define SPI_FSIZE_13 SPI_CTAR_FMSZ(12)
#define SPI_FSIZE_14 SPI_CTAR_FMSZ(13)
#define SPI_FSIZE_15 SPI_CTAR_FMSZ(14)
#define SPI_FSIZE_16 SPI_CTAR_FMSZ(15)

/** Constants to double the clock rate */
#define SPI_DOUBLE_BAUD_RATE 1
#define SPI_NON_DOUBLE_BAUD_RATE 0

/** Constant to configure the baud rate */
#define SPI_BAUD_RATE_2     0x00
#define SPI_BAUD_RATE_4     0x01
#define SPI_BAUD_RATE_6     0x02
#define SPI_BAUD_RATE_8     0x03
#define SPI_BAUD_RATE_16    0x04
#define SPI_BAUD_RATE_32    0x05
#define SPI_BAUD_RATE_64    0x06
#define SPI_BAUD_RATE_128   0x07
#define SPI_BAUD_RATE_256   0x08
#define SPI_BAUD_RATE_512   0x09
#define SPI_BAUD_RATE_1024  0x0A
#define SPI_BAUD_RATE_2048  0x0B
#define SPI_BAUD_RATE_4096  0x0C
#define SPI_BAUD_RATE_8192  0x0D

/** Constant that used to select a chip select */
#define SPI_PCSIS_0 0x100
#define SPI_PCSIS_1 0x200
#define SPI_PCSIS_2 0x400
#define SPI_PCSIS_3 0x800
#define SPI_PCSIS_4 0x1000
#define SPI_PCSIS_5 0x2000


/*Type for FIFO configuration*/
typedef enum {SPI_DISABLE_FIFO, SPI_ENABLE_FIFO} SPI_EnableFIFOType;
/*Type for polarity configuration*/
typedef enum {SPI_LOW_POLARITY,SPI_HIGH_POLARITY} SPI_PolarityType;
/*Type for phase configuration*/
typedef enum {SPI_LOW_PHASE,SPI_HIGH_PHASE} SPI_PhaseType;
/*Type for the star of the frame, either MSB o LSM */
typedef enum {SPI_MSB, SPI_LSM} SPI_LSMorMSBType;
/*Type for SPI modules*/
typedef enum {SPI_0,
			  SPI_1,
			  SPI_2
			  } SPI_ChannelType;
/*Type for master or slave configuration*/			  
typedef enum{SPI_SLAVE,SPI_MASTER} SPI_MasterType;			  
/*Type for GPIO configuration that activates the GPIO for SPI*/
typedef struct{
	GPIO_portNameType GPIO_portName;
	uint32 SPI_clk;
	uint32 SPI_Sout;
}GPIOForSPIType;
/*Type that is used for SPI configuration, It contains all the information needed for a SPI module*/
typedef struct
{
	SPI_EnableFIFOType SPI_EnableFIFO;
	SPI_PolarityType SPI_Polarity;
	SPI_PhaseType SPI_Phase;
	SPI_LSMorMSBType SPI_LSMorMSB;
	SPI_ChannelType SPI_Channel;
	SPI_MasterType SPI_Master;		
	GPIO_pinControlRegisterType pinConttrolRegisterPORTD;
	uint8 baudrate;
	uint32 frameSize;
	GPIOForSPIType GPIOForSPI;
} SPI_ConfigType;


/*!
 * \brief This function enable the clock module of the SPI by modifying the MDIS bits.
 * \param[in] channel SPI Channel to be modified
 * \return void
 */
static void SPI_enable(SPI_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief This function activates the clock gating of the SPI channel
 * \param[in] channel SPI Channel to be modified
 * \return void
 */
static void SPI_clk(SPI_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It configures the SPI as a master or slave depending on the value of masterOrslave
 * \param[in] channel SPI Channel to be modified
 * \param[in] masterOrSlave Defines if its gonna act as a master or a slave
 * \return void
 */
static void SPI_setMaster(SPI_ChannelType channel, SPI_MasterType masterOrSlave);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It activates the TX and RX FIFOs of the SPI depending on the value of enableOrdisable
 * \param[in] channel SPI Channel to be modified
 * \param[in] enableOrDisable Toogles between enabled or disabled for the Tx and Rx FIFO's
 * \return void
 */
static void SPI_FIFO(SPI_ChannelType channel, SPI_EnableFIFOType enableOrDisable);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It selects the clock polarity depending on the value of cpol
 * \param[in] channel SPI Channel to be modified
 * \param[in] cpol Selects the polarity behaviour
 * \return void
 */
static void SPI_ClockPolarity(SPI_ChannelType channel, SPI_PolarityType cpol);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It selects the frame size depending on the value of frameSize and the macros that
 * are defined above. It clears the current frameSize before setting a new one due to problems
 *  with the compiler.
 * \param[in] channel SPI Channel to be modified
 * \param[in] frameSize Establishes the size of the frameSize
 * \return void
 * */
static void SPI_frameSize(SPI_ChannelType channel, uint32 frameSize);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It selects the clock phase depending on the value of cpha.
 * \param[in] channel SPI Channel to be modified
 * \param[in] cpha Toogles the phase selector
 * \return void
 */
static void SPI_ClockPhase(SPI_ChannelType channel, SPI_PhaseType cpha);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It selects the baud rate depending on the value of baudRate and the macros that
 * are defined above
 * \param[in] channel SPI Channel to be modified
 * \param[in] baudRate Pre-established baudRate defined on macros above
 * \return void
 */
static void SPI_baudRate(SPI_ChannelType channel, uint32 baudRate);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It selects if MSB or LSM bits is first transmitted
 * \param[in] channel SPI CHannel to be modified
 * \param[in] msb It selects wich bit is transmitted: 0=MSB 1=LSB
 * \return void
 */
static void SPI_mSBFirst(SPI_ChannelType channel, SPI_LSMorMSBType msb);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It stars the SPI transmission by modifying the value of HALT bit
 * \param[in] channel SPI Channel to be modified
 * \return void
 */
void SPI_startTranference(SPI_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It stops the SPI transmission by modifying the value of HALT bit
 * \param[in] channel SPI Channel to be modified
 * \return void
 */
void SPI_stopTranference(SPI_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 * \brief It transmits the information contained in data. It writes the low part of the PUSHR
 * to start the transmission and clock generation; then it verifies that the tx was made
 * succesfuly by verifying that TFC bit from SPIx_SR changes from 0 to 1. Finally, it cleans the
 * completed transmission flag, by writing a 1 in the SPIx_SR.
 * \param[in] Data Data to be transfered
 */
void SPI_sendOneByte(uint8 Data);

/*It configures the SPI for transmission, this function as arguments receives a pointer to a constant structure where are all 
 * the configuration parameters*/
void SPI_init(const SPI_ConfigType* Configuration);

#endif /* SPI_H_ */
