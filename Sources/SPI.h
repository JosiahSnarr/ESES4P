/*************************************************************************
SPI.h

Author: Josiah Snarr

Date: April 9, 2015

SPI.h contains macros, constants, and prototypes for the SPI functions
  within SPI.c
*************************************************************************/
#include "macros.h"

//Definitions
#define SPICR1_SETUP  0b01010000  //SPI enable, master mode, active high, first edge halfway, MSB first
#define SPIBR_SETUP   0b00000011  //Baud rate of 1Mhz divided from 16Mhz  

#define LSB_FRAME     0           //Frame for LSB set of data to DAC
#define DAC_COM_LDA   0b00010000  //Load input register A on DAC, others unchanged
#define DAC_COM_LDB   0b00100000  //Load input register B on DAC, other unchanged
#define DAC_COM_LDAB  0b10000000  //Load both registers from input, update output
#define DAC_COM_STAN  0b11000000  //Made DAC enter standby
#define DAC_COM_WAKE  0b11010000  //Retunr from standby/shutdown, output from shift registers
#define DAC_COM_SHUT  0b11100000  //Make DAC enter shutdown mode

#define COM_LDAB  0   //Commands for DAC
#define COM_LDA   1
#define COM_LDB   2

//Prototypes
void DACWake(void);
void DACShutdown(void);
void DACStandby(void);
void DACSend(char dVal, char inst);
void putcSPI(char val);
void spiInit(void);