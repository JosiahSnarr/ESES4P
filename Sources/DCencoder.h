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

#define FEEDBACK_SCALE_FACTOR   16777216
#define SENSOR_GAIN             4946

//Gains for feedback control
#define P_GAIN                  25
#define I_GAIN                  25
#define GAIN_DIVISOR            100

//Prototypes
extern void encoderInit(void);
extern void encoderStart(void);
extern void encoderStop(void);
extern void setSpeed(char unsigned speedVal);
extern void encoderVals(void);
extern void stopFeed(void);
extern char unsigned newAValue(void);
extern char unsigned newBValue(void);
extern long unsigned gemmeValBYo(void);
extern long unsigned gemmeValAYo(void);