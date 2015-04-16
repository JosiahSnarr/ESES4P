/*************************************************************************
timer.c

Author: Josiah Snarr

Date: March 28, 2015

timer.c contains functions to support and make use of the HCS12's timer
  module
*************************************************************************/

#include "timer.h"
#include "macros.h"

void timer_init(void)
{
	TIMER_SET_PRESCALER(TIMER_PRESCALER_64);

	TIMER_DISABLE_ON_FREEZE();
	TIMER_FANCY_FAST_CLEAR();
	
	TIMER_ENABLE();
	CLR_BITS(TIE, (TIE_C3I_MASK));
}

void msDelay(int waitTime)
{
  int count; //Count for waiting
  
  TC3 = TCNT + 125; //1ms delay
  
  MAKE_CHNL_OC(TIOS_IOS3_MASK);  //Make output compare
  
  for(count = waitTime; count >= 0; --count)
  {
    while(!(TFLG1&TFLG1_C3F_MASK));  //Wait for timer channel event
    TC3 += 125;                   //Add on 125ms delay
  }
  
  MAKE_CHNL_IC((unsigned char)TIOS_IOS3_MASK); //No more OC for TC3
}
