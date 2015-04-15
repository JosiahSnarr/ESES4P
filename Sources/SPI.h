/*************************************************************************
SPI.h

Author: Josiah Snarr

Date: April 9, 2015

SPI.h contains macros, constants, and prototypes for the SPI functions
  within SPI.c
*************************************************************************/
#include "macros.h"

//Definitions
#define SPI_PORT      PTM
#define SPI_DDR       DDRM
#define SPI_SS        PTM_PTM3_MASK

#define SPICR1_SETUP  0b01010000  //SPI enable, master mode, active high, first edge halfway, MSB first
#define SPIBR_SETUP   0b00000111  //Baud rate of 31250Hz divided from 8Mhz (/256)

#define LSB_FRAME     0           //Frame for LSB set of data to DAC
#define DAC_COM_LDA   0b00010000  //Load input register A on DAC, others unchanged
#define DAC_COM_LDB   0b00100000  //Load input register B on DAC, other unchanged
#define DAC_COM_LDAB  0b11110000  //Load both registers from input, update output
#define DAC_COM_STAN  0b11001100  //Made DAC enter standby with 4V reference
#define DAC_COM_WAKE  0b11011100  //Retunr from standby/shutdown, output from shift registers with 4V reference
#define DAC_COM_SHUT  0b11101100  //Make DAC enter shutdown mode with 4V reference
#define DAC_COM_UPDA  0b11110000  //Update outputs

#define COM_LDAB  0   //Commands for DAC
#define COM_LDA   1
#define COM_LDB   2

//Prototypes
void DACUpdate(void);
void DACWake(void);
void DACShutdown(void);
void DACStandby(void);
void DACSend(unsigned char dVal, unsigned char inst);
void putcSPI(unsigned char val);
void putSPI(unsigned char val);
void spiInit(void);