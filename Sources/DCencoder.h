/*************************************************************************
DCencoder.h

Author: Josiah Snarr

Date: April 15, 2015

DCencoder.h contains macros, constants, and prototypes for the DC motor
  encoders
*************************************************************************/
#include "macros.h"

//Definitions
#define ENCODER_PORT  PTT
#define ENCODER_DDR   DDRT
#define ENCODER_A     PTT_PTT0_MASK
#define ENCODER_B     PTT_PTT1_MASK
#define ENCODER_BOTH  0x03

#define TIMER_A       TC0
#define TIMER_B       TC1

#define TOI_MASK      0x80

#define TRACKING_FLAG 0x01    //When interrupts are active on encoders
#define NEW_FLAG      0x02    //When another second is available
#define CALC_FLAG     0x04    //When another value needs to be calculated

#define NUM_TRACKS    20

#define FEEDBACK_SCALE_FACTOR   1048576
#define SENSOR_GAIN             309

#define FIVE_MS                 625         //625*8nsPerTclk = 5ms
#define MAX_SIGNED_CHAR         127
#define MIN_SIGNED_CHAR         -127

#define STUPID_HIGH_SPEED       10197
#define STUPID_LOW_SPEED        4944

//Prototypes
extern char unsigned gimErr(void);    //!!!DEBUG DEBUG !!!
extern void encoderInit(void);
extern void watchdogInit(void);
extern void encoderStart(void);
extern void watchdogStart(void);
extern void encoderStop(void);
extern void setSpeed(char unsigned speedVal);
extern void encoderVals(void);
extern void stopFeed(void);
extern char unsigned newAValue(void);
extern char unsigned newBValue(void);
extern long unsigned gemmeValBYo(void);
extern long unsigned gemmeValAYo(void);