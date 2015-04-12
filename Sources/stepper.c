/*************************************************************************
stepper.c

Author: Josiah Snarr

Date: April 11, 2015

stepper.c contains the functions to control the stepper motor on the
  camera mount
*************************************************************************/
#include "derivative.h"
#include "stepper.h"
#include "timer.h"
#include "LCD_Macros.h"

//Motor positions to run through
static const char motorPos[STEP_NUM_POS] =
{
  0b10000000,
  0b10100000,
  0b00100000,
  0b01100000,
  0b01000000,
  0b01010000,
  0b00010000,
  0b10010000
};

static volatile signed   char direction;  //Direction for motor to turn
static volatile unsigned int numSteps;   //Number of steps for stepper motor
static volatile unsigned int currStep;   //Current step the motor is on
static volatile unsigned int moveStep;   //Step to move to
static volatile unsigned char stepIndex;  //Current index into motorPosition array
static volatile unsigned char isBusy;     //Busy flag for stepper motor

/*************************************************************************
Author: Josiah Snarr
Date: April 11, 2015

stepperInit initializes the ports which the stepper motor uses, PORTT and
  PORTAD
*************************************************************************/
void stepperInit(void)
{
  SET_BITS(STEP_DDR, STEP_OUT);
  SET_BITS(SMEN_DDR, SMEN_MASK);
  SET_BITS(STEP_SWI_DIS, STEP_SWI_IN);
  CLR_BITS(STEP_SWI_DDR, STEP_SWI_IN);
  
  MAKE_CHNL_OC(TIOS_IOS4_MASK);
  TIE &= (~((unsigned char)TIOS_IOS4_MASK)); //Disable interrupts
  SET_BITS(SMEN_PORT, SMEN_MASK);
  
  //Initial direction left, and start at beginning of table
  direction = DIR_LEFT;
  stepIndex = 0;
  
  stepperHome();
}

/*************************************************************************
Author: Josiah Snarr
Date: April 11, 2015

stepperStep controls the stepper motor to step a position based on the
  direction it is going
*************************************************************************/
void stepperStep(void)
{
  //Step to next position in table
  FORCE_BITS(STEP_PORT, STEP_OUT, motorPos[stepIndex]);
  
  //Set index to next position
  stepIndex = (stepIndex + direction) & INDEX_MASK;
  
  //Increase/decrease current step
  currStep += direction;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 11, 2015

stepperHome finds the stepper limits on the platform, and centers the
  stepper at the midpoint between them
*************************************************************************/
void stepperHome(void)
{
  unsigned int midpoint;

  //Delay stops it from going "ttttttzzzhhggggghhghhzzzzzttttt"
  msDelay(25);

  //Go fully left
  while(IS_CLR(STEP_SWI_PORT, SWI_LEFT))
  {
    stepperStep();
    msDelay(2);
  }
  
  //All the way left, 0-position, switch direction
  currStep = 0;
  SWI_DIR(direction);

  //Go all the way right
  while(IS_CLR(STEP_SWI_PORT, SWI_RIGHT))
  {
    stepperStep();
    msDelay(2);
  }
  
  //All the way right, number of steps is current step, find midpoint and switch direction
  numSteps = currStep;
  midpoint = currStep/2;
  SWI_DIR(direction);

  //Move to midpoint
  while(currStep != midpoint)
  {
    stepperStep();
    msDelay(2);
  }
}

/*************************************************************************
Author: Josiah Snarr
Date: April 11, 2015

stepperPosition returns the position of the stepper motor
*************************************************************************/
unsigned int stepperPosition(void)
{
  unsigned int returnVal;
  
  //Find step position, protect form interrupt access
  DisableInterrupts;
  returnVal = currStep;
  EnableInterrupts;
  
  return(currStep);
}

/*************************************************************************
Author: Josiah Snarr
Date: April 11, 2015

stepperBusy returns whether or not the stepper motor is busy
*************************************************************************/
unsigned int stepperBusy(void)
{
  unsigned int returnVal;
  
  //Copy busy flag, protect from interrupts
  DisableInterrupts;
  returnVal = isBusy;
  EnableInterrupts;
  
  return(returnVal);

}

/*************************************************************************
Author: Josiah Snarr
Date: April 11, 2015

stepperAngle moves the stepper motor to a specialized angle
*************************************************************************/
void stepperAngle(unsigned char angle)
{
  angle %= MAX_ANGLE; //Ensure angle is < 180°
  
  moveStep = (unsigned int)((angle*numSteps)/MAX_ANGLE);  //Find where to move to
  
  //Find direction to move
  if(moveStep > currStep)
  {
    direction = DIR_RIGHT;
  }
  else
  {
    direction = DIR_LEFT;
  }
  
  //Prepare interrupts for movement, enable interrupts, and set busy flag
  TC4 = TCNT + STEP_WAIT;
  TIE|= TIOS_IOS4_MASK;
  isBusy = BUSY;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 11, 2015

the stepControl interrupt controls the next step on the stepper motor, and
  resets the busy flag when it is complete
*************************************************************************/
interrupt VectorNumber_Vtimch4 void stepControl(void)
{
  if(currStep != moveStep)
  {
    //Step to next position in table
    FORCE_BITS(STEP_PORT, STEP_OUT, motorPos[stepIndex]);
  
    //Set index to next position
    stepIndex = (stepIndex + direction) & INDEX_MASK;
  
    //Increase/decrease current step
    currStep += direction;
    
    //Increase timer for next interrupts
    TC4 += STEP_WAIT;
  }
  else
  {
    //Clear busy flag, disable TC4 interrupts
    isBusy = NOT_BUSY;
    TIE &= (~((unsigned char)TIOS_IOS4_MASK));
  }
}