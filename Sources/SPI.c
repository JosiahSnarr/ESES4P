/*************************************************************************
SPI.c

Author: Josiah Snarr

Date: April 9, 2015

SPI.c contains the functions to control the SPI interface and DAC
*************************************************************************/
#include "derivative.h"
#include "SPI.h"


/*************************************************************************
Author: Josiah Snarr

Date: April 9, 2015

DACWake wakes the DAC
*************************************************************************/
void DACWake(void)
{
  putcSPI(DAC_COM_WAKE);
  putcSPI(LSB_FRAME);
}

/*************************************************************************
Author: Josiah Snarr

Date: April 9, 2015

DACShutdown shuts down the DAC
*************************************************************************/
void DACShutdown(void)
{
  putcSPI(DAC_COM_SHUT);
  putcSPI(LSB_FRAME);
}

/*************************************************************************
Author: Josiah Snarr

Date: April 9, 2015

DACStandby puts the DAC on standby
*************************************************************************/
void DACStandby(void)
{
  putcSPI(DAC_COM_STAN);
  putcSPI(LSB_FRAME);
}

/*************************************************************************
Author: Josiah Snarr

Date: April 9, 2015

DACSend forms a header for data sent to the DAC, and sends the data with
  the specified instruction (0 - LDAB, 1 - LDA, 2 - LDB)
*************************************************************************/
void DACSend(char dVal, char inst)
{
  char lss;     //Least significant set to send
  char mss;     //Most significant set to send
  
  switch(inst)    //Find command and form MSB side of header
  {
    case COM_LDA:
      mss = (DAC_COM_LDA | (HI_NYBBLE(dVal)));
      break;
    case COM_LDB:
      mss = (DAC_COM_LDB | (HI_NYBBLE(dVal)));
      break;
    case COM_LDAB:
      mss = (DAC_COM_LDAB | (HI_NYBBLE(dVal)));
      break;
    default:
      break;
  }
  
  lss = (NYB_CAT(dVal, LSB_FRAME)); //Form LSB side of header
  
  //Push output to DAC
  putcSPI(mss);
  putcSPI(lss);
}

/*************************************************************************
Author: Josiah Snarr

Date: April 9, 2015

putcSPI pushes a byte-long value through the SPI interface
*************************************************************************/
void putcSPI(char val)
{
  char tempVal; //Variable to throw return to
  
  while(!(SPISR & SPISR_SPTEF_MASK));   //Wait until write permissible
  SPIDR = val;                          //Output byte
  
  while(!(SPISR & SPISR_SPIF_MASK));    //Wait until write complete
  tempVal = SPIDR;                      //Clear flag
}

/*************************************************************************
Author: Josiah Snarr

Date: April 9, 2015

spiInit initializes the SPI for 1MHz baud and no slave select
*************************************************************************/
void spiInit(void)
{
  SPICR1 = SPICR1_SETUP;
  SPIBR = SPIBR_SETUP;
}