/*************************************************************************
acclerometer.c

Author: Josiah Snarr

Date: April 8, 2015

accelerometer.c contains the functions to control the accelerometer
*************************************************************************/
#include "accelerometer.h"
#include "timer.h"

/*************************************************************************
Author: Josiah Snarr

Date: April 8, 2014

accInit initializes the ports used as GPIOs for the accelerometer
  direction select.
*************************************************************************/
void accInit(void)
{
  //Set up input for channel select
  SET_BITS(ACC_FUNC_DDR, ACC_FUNC_BOTH_MASK);
  
  //Set up ATDCTL2 for power up + fast clear
  ATDCTL2 = ATDCTL2_SETUP;
  
  //Set up ATDCTL3 for 1 conversion, FIFO off, and finish-then-freeze
  ATDCTL3 = ATDCTL3_SETUP;
  
  //Set up ATDCTL4 for 10-bit data, 2 sample clocks, prescaler of 8(16MHz/8=2MHz)
  ATDCTL4 = ATDCTL4_SETUP;
}

/*************************************************************************
Author: Josiah Snarr

Date: April 8, 2014

getAccValue gets an accelerometer reading for the specified channel
*************************************************************************/
unsigned int getAccValue(char channel)
{
  unsigned int value = 0;
  
  setAccChnl(channel);
  msDelay(100);
  //Set up ATDCTL5 for right justified and channel 0
  ATDCTL5 = ATDCTL5_SETUP;
  
  //Wait for conversion to be complete
  while(((ATDSTAT0 & ATDSTAT0_SCF_MASK) && ATDSTAT0_SCF_MASK) != 0);
  
  //Clear flag, read result (fast clear, reading clears SCF flag)
  value = ATDDR0;

  return(value);  //Acceleration value
}

/*************************************************************************
Author: Josiah Snarr

Date: April 8, 2014

setAccChnl sets the channel that the accelerometer is reading from
*************************************************************************/
void setAccChnl(char channel)
{
  //Force cleare both bits (selects x)
  ACC_SEL_X;
  
  switch(channel)
  {
    case 1:
      //Nothing to do, x selected (both bits clear)
      break;
      
    case 2:
      //Set S0 for y select
      ACC_SEL_Y;
      break;
      
    case 3:
      //Set S1 for z select
      ACC_SEL_Z;
      break;
      
    default:
      break;
  }
}