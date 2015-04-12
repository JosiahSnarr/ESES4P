/*************************************************************************
servo.c

Author: Josiah Snarr

Date: March 30, 2015

servo.c contains the functions and interrupts to control the servo motor
*************************************************************************/

#include "servo.h"
#include "timer.h"

static volatile unsigned int riseFallO  = TIMER_OC_ACTION_DRIVE_HIGH;       //Rising or falling edge set (output)
static volatile unsigned int pulseWidth = 0;    //Value of the outgoing pulse width on PT2
static volatile unsigned int offTime    = 0;    //Value of off time for output

void servo_init(void)
{
  DisableInterrupts;  //Set up everything in critical section
  
  //Initialize servo to be at 0°
  pulseWidth = (INIT_TIME+OFFSET)*OC_DELTA_10US;
  pulseWidth += pulseWidth>>2;   //Make up for the .25 error
  
  offTime = TIMER_OFF_TIME - pulseWidth;
  offTime += offTime>>2;
  
  //Configure TC2 as OC
  TIOS |= (TIOS_IOS2_MASK);
  
  //Set up output capture for the servo motor
  TC2   = TCNT + pulseWidth;    //Set up TC2 for first OC event
  TIE   |= TIE_C2I_MASK;        //Enable TC2 for interrupts
  TFLG1 = TFLG1_C2F_MASK;       //Clear any pending events on TC2
  
  SET_OC_ACTION(2, riseFallO);  //Set output compare for servo
  
  EnableInterrupts;   //Critical section complete, re-enable interrupts
}

void servo_angle(unsigned int angle)
{
  if(angle <= 180){
    DisableInterrupts;  //Critical section (use of pulseWidth)
    
#if OC_DELTA_10US == 1
    pulseWidth = (angle+OFFSET);//*OC_DELTA_10US; if OC_DELTA_10US not 1
#else
#error "Wrong prescaler"
#endif

    pulseWidth += pulseWidth>>2;   //Make up for the .25 error
    
    offTime = TIMER_OFF_TIME - pulseWidth;
    offTime += offTime>>2;
    
    EnableInterrupts;   //End of critical section (use of pulseWidth)
  }
}

interrupt VectorNumber_Vtimch2 void servo_pulse(void)
{
  unsigned int timeCapt = TC2;  //Capture TCNT time at interrupt fire
  
  //Set up the next interrupt to time of interrupt + width of pulse or offtime
  switch(riseFallO)
  {
      case TIMER_OC_ACTION_DRIVE_HIGH:
        //Set high for length of high pulse width
        riseFallO = TIMER_OC_ACTION_DRIVE_LOW;
        TC2 = timeCapt + pulseWidth;
        break;
      case TIMER_OC_ACTION_DRIVE_LOW:
        //Set low for length of off time
        riseFallO = TIMER_OC_ACTION_DRIVE_HIGH;
        TC2 = timeCapt + offTime;
        break;
  }
  SET_OC_ACTION(2, riseFallO);  //Set output compare for servo
}