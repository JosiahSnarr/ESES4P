/*************************************************************************
DCencoder.c

Author: Josiah Snarr

Date: April 13, 2015

DCencoder.c contains the functions to control the DC motor speed and
  encoder feedback
*************************************************************************/
#include "derivative.h"
#include "DCencoder.h"
#include "timer.h"

long  volatile static unsigned encoderASecond[NUM_TRACKS];      //Encoder A ticks over last second
char  volatile static unsigned currASec;            //Current second of A in buffer
int   volatile static unsigned encoderAO;            //Encoder A overflow
int   volatile static unsigned encoderAFirstCapt;    //Encoder A first TCNT capture
int   volatile static unsigned encoderALastCapt;     //Encoder B last TCNT capture
char  volatile static unsigned AFlag;               //Flag for encoder A status

long  volatile static unsigned encoderBSecond[NUM_TRACKS];      //Encoder B ticks over last second
char  volatile static unsigned currBSec;            //Current second of B in buffer
int   volatile static unsigned encoderBO;            //Encoder B overflow
int   volatile static unsigned encoderBFirstCapt;    //Encoder B first TCNT capture
int   volatile static unsigned encoderBLastCapt;     //Encoder B last TCNT capture
char  volatile static unsigned BFlag;               //Flag for encoder B status

/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

encoderInit initializes the DC motor encoder on port T
*************************************************************************/
void encoderInit(void)
{
  CLR_BITS(ENCODER_DDR, ENCODER_BOTH);
  MAKE_CHNL_IC((unsigned char)TIOS_IOS1_MASK);    //Encoder B input capture
  MAKE_CHNL_IC((unsigned char)TIOS_IOS0_MASK);    //Encoder A input capture
  
  SET_IC_ACTION(0, TIMER_IC_ACTION_NO_ACTION);    //Encoder A no interrupt for now
  SET_IC_ACTION(1, TIMER_IC_ACTION_NO_ACTION);    //Encoder B no interrupt for now
  
  CLR_BITS(TIE, (TIE_C1I_MASK|TIE_C0I_MASK));     //Disable interrupts for both encoders
  CLR_BITS(TSCR2, TOI_MASK);                      //Disable overflow interrupt
  
  AFlag = 0;
  BFlag = 0;
  currASec = 0;
  currBSec = 0;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

encoderStart starts the encoders to track DC motor movement
*************************************************************************/
void encoderStart(void)
{
  //Initialize variables
  encoderAFirstCapt = TCNT;
  encoderBFirstCapt = TCNT;
  AFlag = 0x01;
  BFlag = 0x01;
  
  //Set up channels for rising edge interrupts
  SET_IC_ACTION(0, TIMER_IC_ACTION_RISING_EDGE);
  SET_IC_ACTION(1, TIMER_IC_ACTION_RISING_EDGE);
  
  SET_BITS(TIE, (TIE_C1I_MASK|TIE_C0I_MASK));     //Enable interrupts for both encoders
  SET_BITS(TSCR2, TOI_MASK);                      //Enable overflow interrupt
}

/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

encoderStop gracefully clears all variables and disables interrupts
*************************************************************************/
void encoderStop(void)
{
  CLR_BITS(TIE, (TIE_C1I_MASK|TIE_C0I_MASK));     //Disable interrupts for both encoders
  CLR_BITS(TSCR2, TOI_MASK);                      //Disable overflow interrupt
  
  //Set flags to 0
  AFlag = 0;
  BFlag = 0;
}

long gemmeValBYo(void)
{
  return encoderBSecond[currASec-1];
}

long gemmeValAYo(void)
{
  return encoderASecond[currASec-1];
}

/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

encoderAHandler handles interrupts for encoder A
*************************************************************************/
interrupt VectorNumber_Vtimch0 void encoderAHandler(void)
{
  encoderALastCapt = TIMER_A;   //Get time at interrupt

  if(encoderAFirstCapt < encoderALastCapt)  //If extra overflow count, remove it
  {
    encoderAO--;
  }
  
  //Calculate total time passed
  encoderASecond[currASec] = (encoderAO*MAX_INT)+(encoderALastCapt - encoderAFirstCapt);
  
  //Store captures, increment array
  encoderAFirstCapt = encoderALastCapt;
  currASec = (char unsigned)((currASec + 1) % NUM_TRACKS);
}

/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

encoderBHandler handles interrupts for encoder B
*************************************************************************/
interrupt VectorNumber_Vtimch1 void encoderBHandler(void)
{
  encoderBLastCapt = TIMER_B;   //Get time at interrupt
  
  if(encoderBFirstCapt < encoderBLastCapt)  //If extra overflow count, remove it
  {
    encoderBO--;
  }
  
  //Calculate total time passed
  encoderBSecond[currBSec] = (encoderBO*MAX_INT)+(encoderBLastCapt - encoderBFirstCapt);
  
  //Store captures, increment array
  encoderBFirstCapt = encoderBLastCapt;
  currBSec = (char unsigned)((currBSec + 1) % NUM_TRACKS);
}


/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

overflowHandler handles timer channel overflows

NOTE **POSSIBLE FUTURE IMPLEMENTATION, WORTH 1%**
*************************************************************************/
interrupt VectorNumber_Vtimpaovf void overflowHandler(void)
{
  encoderAO++;
  encoderBO++;
}