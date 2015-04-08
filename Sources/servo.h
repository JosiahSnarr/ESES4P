/*************************************************************************
servo.h

Author: Josiah Snarr

Date: March 30, 2015

servo.h contains macros, constants, and prototypes for the servo control
  of the HCS12 embedded controller project.
*************************************************************************/

#ifndef SERVO_H
#define SERVO_H

#include <hidef.h>
#include "derivative.h"

#define OC_DELTA_10US     1         // 0.01ms delta with prescaler of 64 (actually 1.25)
#define PRE_ERR           4         // for every 4 counts, should be another 1 (1.25*4=5)
#define TIMER_OFF_TIME    ((2000 * OC_DELTA_10US)) //Off time for servo (20ms)
#define INIT_TIME         90        // initially 1500us pulse
#define OFFSET            60        // offset to make pulse from angle input

void servo_init(void);

void servo_angle(unsigned int angle);

interrupt VectorNumber_Vtimch2 void servo_pulse(void);

#endif