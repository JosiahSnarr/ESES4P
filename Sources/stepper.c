/*************************************************************************
stepper.c

Author: Josiah Snarr

Date: April 11, 2015

stepper.c contains the functions to control the stepper motor on the
  camera mount
*************************************************************************/
#include "derivative.h"
#include "stepper.h"

//Motor positions to run through
static const char motorPos[STEP_NUM_POS] =
{
  0b00010000,
  0b00110000,
  0b00100000,
  0b01100000,
  0b01000000,
  0b11000000,
  0b10000000,
  0b10010000
};

static volatile char signed   direction; //Direction for motor to turn
static volatile char unsigned numSteps;  //Number of steps for stepper motor
static volatile char unsigned currStep;  //Current step the motor is on
static volatile char unsigned stepIndex; //Current index into motorPosition array

/*************************************************************************
Author: Josiah Snarr
Date: April 11, 2015

stepperInit initializes the ports which the stepper motor uses, PORTT and
  PORTAD
*************************************************************************/
void stepperInit(void)
{
  SET_BITS(STEP_DDR, STEP_OUT);
  SET_BITS(STEP_SWI_DIS, STEP_SWI_IN);
  CLR_BITS(STEP_SWI_DDR, STEP_SWI_IN);
  direction = DIR_LEFT;
  stepIndex = 0;
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
  while(!IS_CLR(STEP_SWI_PORT, SWI_LEFT));
}