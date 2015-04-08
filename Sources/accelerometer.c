/*************************************************************************
acclerometer.c

Author: Josiah Snarr

Date: April 8, 2015

accelerometer.c contains the functions to control the accelerometer
*************************************************************************/
#include "accelerometer.h"
#include "derivative.h"

/*************************************************************************
Author: Josiah Snarr

Date: April 8, 2014

accInit initializes the ports used as GPIOs for the accelerometer
  direction select.
*************************************************************************/
void accInit(void)
{
  SET_BITS(ACC_FUNC_DDR, ACC_FUNC_BOTH_MASK);
}

/*************************************************************************
Author: Josiah Snarr

Date: April 8, 2014

getAccValue gets an accelerometer reading for the specified channel
*************************************************************************/
unsigned int getAccValue(unsigned char channel)
{
  unsigned int value = 0;
  
  setAccChnl(channel);
  
  //Enable AD0
  //SET_BIT(ATDCTL2, AN0);
  
  return(value);
}

/*************************************************************************
Author: Josiah Snarr

Date: April 8, 2014

setAccChnl sets the channel that the accelerometer is reading from
*************************************************************************/
void setAccChnl(unsigned char channel)
{
  //Force cleare both bits (selects x)
  ACC_SEL_X;
  
  switch(channel)
  {
    case 'x':
      //Nothing to do, x selected (both bits clear)
      break;
      
    case 'y':
      //Set S0 for y select
      ACC_SEL_Y;
      break;
      
    case 'z':
      //Set S1 for z select
      ACC_SEL_Z;
      break;
      
    default:
      break;
  }
}