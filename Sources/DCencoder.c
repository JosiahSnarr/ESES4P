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

int   static unsigned fsf = 2^FEEDBACK_SCALE_POWER; //Feedback scale factor

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
  
  SET_BITS(TIE, TIE_C0I_MASK);//(TIE_C1I_MASK|TIE_C0I_MASK));     //Enable interrupts for both encoders
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

/*************************************************************************
Author: Josiah Snarr
Date: April 16, 2015

speedJa returns the speed of a motor
*************************************************************************/
int speedJa(char motorJa)
{
  int unsigned lastCapt;      //Most recent tclk capture taken
  int unsigned firstCapt;     //First tclk capture taken
  int unsigned tclkCount;     //Amount of tclk counts passed
  int unsigned encPer;        //Encoder period (ns)
  int unsigned encSpeed;      //Speed of the encoder shaft  (hz)
  int unsigned encShaftSpeed; //Encoder shaft speed (hz)(x10)
  int unsigned motShaftSpeed; //Motor shaft speed (hz)(x10)
  int unsigned speed;         //Speed of the motor
  
  switch(motorJa)
  {
    case 'A': //If motor A, get A captures
      DisableInterrupts;
      lastCapt = encoderALastCapt;
      firstCapt = encoderAFirstCapt;
      EnableInterrupts;
      break;
    case 'B': //If motor B, get B captures
      DisableInterrupts;
      lastCapt = encoderBLastCapt;
      firstCapt = encoderBFirstCapt;
      EnableInterrupts;
      break;
    default:
      //Nothing here ja
      break;
  }
  
  //Get amount of tclk counts and from that the encoder period, and then speed (hz)
  tclkCount = firstCapt - lastCapt;
  encPer = NS_PERIOD*tclkCount;
  encSpeed = (int unsigned)(((long unsigned)(encPer*1000000))/NUM_ENCODER_VANES);   //*1000000 to get into seconds
  
  //Get the motor encoder shaft speed in hz (x10)
  encShaftSpeed = (encSpeed*10)/GEAR_RAT_X_10;
  
  //Get the motor output shaft speed in hz and make up for the digit 4 in pi for more accuracy
  motShaftSpeed = (encShaftSpeed)*(PI_10*CIRC_10);
  motShaftSpeed += (2*motShaftSpeed)/5;
  
  //Reverse previous *10's, this is speed
  speed = motShaftSpeed/10;
  
  return speed;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 16, 2015

calculateA does calculations on encoder A's values so that they need not
  be done in the interrupt
*************************************************************************/
void calculateA(void)
{
  if((AFlag & CALC_FLAG) == CALC_FLAG)
  {  
    if((encoderAFirstCapt < encoderALastCapt) && encoderAO != 0)  //If extra overflow count, remove it
    {
      encoderAO--;
    }
  
    //Calculate total time passed
    encoderASecond[currASec] = (encoderAO*MAX_INT)+(encoderALastCapt - encoderAFirstCapt);
  
    //Store captures, increment array, set new flag, reset overflow
    SET_BITS(AFlag, NEW_FLAG);
    encoderAO = 0;
    currASec = (char unsigned)((currASec + 1) % NUM_TRACKS);
  }
}

/*************************************************************************
Author: Josiah Snarr
Date: April 16, 2015

calculateB does calculations on encoder A's values so that they need not
  be done in the interrupt
*************************************************************************/
void calculateB(void)
{
  if((BFlag & CALC_FLAG) == CALC_FLAG)
  {  
    if((encoderBFirstCapt < encoderBLastCapt) && encoderBO != 0)  //If extra overflow count, remove it
    {
      encoderBO--;
    }
  
    //Calculate total time passed
    encoderBSecond[currBSec] = (encoderBO*MAX_INT)+(encoderBLastCapt - encoderBFirstCapt);
  
    //Store captures, increment array, set new flag, reset overflow
    SET_BITS(BFlag, NEW_FLAG);
    encoderBO = 0;
    currBSec = (char unsigned)((currBSec + 1) % NUM_TRACKS);
  }
}

/*************************************************************************
Author: Josiah Snarr
Date: April 16, 2015

newAValue returns a true/false on whether or not there is a new value
*************************************************************************/
char unsigned newAValue(void)
{
  char unsigned returnVal = ((AFlag & NEW_FLAG)>>1);  //Isolate flag bit
  CLR_BITS(AFlag, NEW_FLAG);
  return(returnVal);
}

/*************************************************************************
Author: Josiah Snarr
Date: April 16, 2015

newBValue returns a true/false on whether or not there is a new value
*************************************************************************/
char unsigned newBValue(void)
{
  char unsigned returnVal = ((BFlag & NEW_FLAG)>>1);  //Isolate flag bit
  CLR_BITS(BFlag, NEW_FLAG);
  return(returnVal);
}

  //DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
long unsigned gemmeValBYo(void)
{ //DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
  return encoderBSecond[currASec-1];
} //DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
  //DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
long unsigned gemmeValAYo(void)
{ //DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
  return encoderASecond[currASec-1];
} //DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
  //DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG

/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

encoderAHandler handles interrupts for encoder A
*************************************************************************/
interrupt VectorNumber_Vtimch0 void encoderAHandler(void)
{
  //Get time at interrupt
  encoderAFirstCapt = encoderALastCapt;
  encoderALastCapt = TIMER_A;
  
  //Store captures, increment array, set new value flag, reset overflow
  SET_BITS(AFlag, CALC_FLAG);
}

/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

encoderBHandler handles interrupts for encoder B
*************************************************************************/
interrupt VectorNumber_Vtimch1 void encoderBHandler(void)
{
  //Get time at interrupt
  encoderBFirstCapt = encoderBLastCapt;
  encoderBLastCapt = TIMER_B;

  //Set newflag/new calculation
  SET_BITS(BFlag, CALC_FLAG);
}


/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

overflowHandler handles timer channel overflows

NOTE **POSSIBLE FUTURE IMPLEMENTATION, WORTH 1%**
*************************************************************************/
interrupt VectorNumber_Vtimovf void overflowHandler(void)
{
  encoderAO++;
  encoderBO++;
}