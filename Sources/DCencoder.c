/*************************************************************************
DCencoder.c

Author: Josiah Snarr

Date: April 13, 2015

DCencoder.c contains the functions to control the DC motor speed and
  encoder feedback
*************************************************************************/
#include "derivative.h"
#include "DCencoder.h"
#include "DCmotor.h"
#include "timer.h"
#include "LCD_Macros.h"

int unsigned P_GAIN= 25;
int unsigned I_GAIN= 25;
int unsigned GAIN_DIVISOR= 100;

//Globals
char  volatile static unsigned currASec;              //Current second of A in buffer
int   volatile static unsigned encoderAO;             //Encoder A overflow
int   volatile static unsigned encoderAFirstCapt;     //Encoder A first TCNT capture
char  volatile static unsigned AFlag;                 //Flag for encoder A status

char  volatile static unsigned currBSec;              //Current second of B in buffer
int   volatile static unsigned encoderBO;             //Encoder B overflow
int   volatile static unsigned encoderBFirstCapt;     //Encoder B first TCNT capture
char  volatile static unsigned BFlag;                 //Flag for encoder B status

char  volatile static unsigned speedFlag;             //Flag for speed being set
int   volatile static unsigned setSREP;               //SREP setpoint
int   volatile static unsigned SREP = 0;
char  volatile static unsigned motorDuty;             //Duty cycle for motor

char  volatile static unsigned errJa;

char unsigned gimErr(void){
  char unsigned returnVal;
  DisableInterrupts;
  returnVal = errJa;
  EnableInterrupts;
  return(returnVal);
}

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
  speedFlag = 0;
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
  //SET_BITS(TSCR2, TOI_MASK);                      //Enable overflow interrupt
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
Date: April 23, 2015

setSpeed sets up the setpoint for the motor feedback
*************************************************************************/
void setSpeed(char unsigned speedVal)
{
  DisableInterrupts;
  
  //Multiply by feedback scale factor to find set point
  setSREP = speedVal * SENSOR_GAIN;
  speedFlag = 0;
  
  EnableInterrupts;
  
  DUTY_BOTH(MAX_DUTY);
  msDelay(200);
  
  DisableInterrupts;
  //Set flag for speed being set
  speedFlag = 1;
  
  EnableInterrupts;
  
  
}

void encoderVals(void)
{
  static int unsigned val2;
  static char unsigned val1;
  
  DisableInterrupts;
  val1 = motorDuty;
  val2 = SREP;
  EnableInterrupts;
  LCDclear();
  LCDprintf("\r\n%u %u", val1, val2);
}

/*************************************************************************
Author: Josiah Snarr
Date: April 23, 2015

stopFeed safely stops the closed loop feedback
*************************************************************************/
void stopFeed(void)
{
  DisableInterrupts;
  speedFlag = 0;
  EnableInterrupts;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 23, 2015

startFeed safely starts the closed loop feedback
*************************************************************************/
void startFeed(void)
{
  DisableInterrupts;
  speedFlag++;
  EnableInterrupts;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

encoderAHandler handles interrupts for encoder A
*************************************************************************/
interrupt VectorNumber_Vtimch0 void encoderAHandler(void)
{
  int static unsigned lastA = 0;                //Last value of timer channel for encoder A
                   //SREP (scaled reciprocal of encoder period)
  int static signed   speedError = 0;           //Speed error
  int static signed   speedErrorIntegral = 0;   //Value for integration
  int static signed   driveValue = 0;           //Drive value for new duty cycle
  //Get time at interrupt
  currASec = TIMER_A - lastA;
  lastA = TIMER_A;
  
  //Check if speed has been set
  if(speedFlag != 0){
    SREP = (int unsigned)(FEEDBACK_SCALE_FACTOR/lastA); //Get scaled reciprocal of encoder period
    speedError = setSREP - SREP;
    errJa = speedError/MAX_DUTY;
    speedErrorIntegral += speedError;
    driveValue = ((speedError * P_GAIN) + (speedErrorIntegral * I_GAIN)) / GAIN_DIVISOR;
    
    //If outside of ranges, make it at the edge
    if(driveValue > MAX_DUTY)
    {
      driveValue = MAX_DUTY;
    }
    else if(driveValue < MIN_DUTY)
    {
      driveValue = MIN_DUTY;
    }
    
    motorDuty = (char unsigned)LOW(driveValue);
    
    DUTY_BOTH(motorDuty);
  }
}

/*************************************************************************
Author: Josiah Snarr
Date: April 15, 2015

encoderBHandler handles interrupts for encoder B
*************************************************************************/
interrupt VectorNumber_Vtimch1 void encoderBHandler(void)
{
  int static unsigned lastA = 0;                //Last value of timer channel for encoder A
                   //SREP (scaled reciprocal of encoder period)
  int static signed   speedError = 0;           //Speed error
  int static signed   speedErrorIntegral = 0;   //Value for integration
  int static signed   driveValue = 0;           //Drive value for new duty cycle
  //Get time at interrupt
  currASec = TIMER_A - lastA;
  lastA = TIMER_A;
  
  //Check if speed has been set
  if(speedFlag != 0){
    SREP = (int unsigned)(FEEDBACK_SCALE_FACTOR/lastA); //Get scaled reciprocal of encoder period
    speedError = setSREP - SREP;
    speedErrorIntegral += speedError;
    driveValue = ((speedError * P_GAIN) + (speedErrorIntegral * I_GAIN)) / GAIN_DIVISOR;
    
    //If outside of ranges, make it at the edge
    if(driveValue > MAX_DUTY)
    {
      driveValue = MAX_DUTY;
    }
    else if(driveValue < MIN_DUTY)
    {
      driveValue = MIN_DUTY;
    }
    
    motorDuty = (char unsigned)LOW(driveValue);
    
    DUTY_BOTH(motorDuty);
  }
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