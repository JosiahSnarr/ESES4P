/*************************************************************************
DCmotor.c

Author: Josiah Snarr

Date: April 13, 2015

DCmotor.c contains the functions to control the DC motor
*************************************************************************/
#include "derivative.h"
#include "DCmotor.h"

/*************************************************************************
Author: Josiah Snarr
Date: April 13, 2015

DCinit initializes the pwm module which the DC motor uses
*************************************************************************/
void DCinit(void)
{
  //Set up PWM
  CLR_BITS(PWMPOL, MOTORS_MASK);      //Duty cycle corresponds to low time
  CLR_BITS(PWMCLK, MOTORS_MASK);      //Select clock A for both motors
  PWMPRCLK = CLOCK_SCALE;             //No prescale
  CLR_BITS(PWMCAE, MOTORS_MASK);      //Left align the pulses
  PWMPER4 = PWM_PER;                  //Motor A period
  PWMPER5 = PWM_PER;                  //Motor B period
  DUTY_A(MIN_DUTY);                   //Motor A is off
  DUTY_B(MIN_DUTY);                   //Motor B is off
  DISABLE_BOTH;                       //Ensure motors are disabled
  
  //Set up port B for motor direction selection
  SET_BITS(DC_DDR, MOTOR_SEL_MASK);   //Set port B pins 0-3 as output
  //Ensure motor is forward direction
  FORWARD_BOTH;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 14, 2015

DCstart starts the DC motors
*************************************************************************/
void DCstart(void)
{
  ENABLE_BOTH;
  DUTY_BOTH(0x10);
}

/*************************************************************************
Author: Josiah Snarr
Date: April 14, 2015

DCstop stops the DC motors
*************************************************************************/
void DCstop(void)
{
  DISABLE_BOTH;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 14, 2015

DCdirec sets the direction of the specified motors
*************************************************************************/
void DCdirec(char mot, char direc)
{
  switch(direc)   //Find direction and go to coresponding selection
  {
    case DIREC_FOR:
      break;
    case DIREC_BAC:
      goto DIRBAC;
      break;
    default:
      goto DONOTHING;
  }
  
  //For forward, once decision made go to end
  switch(mot)
  {
    case 'A':
      FORWARD_A;
      goto DONOTHING;
      break;
    case 'B':
      FORWARD_B;
      goto DONOTHING;
      break;
    case 'C':
      FORWARD_BOTH;
      goto DONOTHING;
      break;
    default:
      goto DONOTHING;
  }
  
  //For reverse, once decision made go to end
  DIRBAC:switch(mot)
  {
    case 'A':
      REVERSE_A;
      break;
    case 'B':
      REVERSE_B;
      break;
    case 'C':
      REVERSE_BOTH;
      break;
    default:
      break;
  }
  
  //Make sure program counter doesn't go on forever if return from interrupt here
  DONOTHING:asm(nop);
}