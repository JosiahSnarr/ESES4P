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

#define FEEDBACK_SCALE_POWER  24    //Feedback scale factor is 2^ this value
#define NUM_ENCODER_VANES     27    //Number of vanes on the encoder
#define GEAR_RAT_X_10         225   //Gear ratio x 10
#define PI_10                 31    //Pi x 10
#define CIRC_10               1665  //10x motor speed

//Prototypes
extern void encoderInit(void);
extern void encoderStart(void);
extern void encoderStop(void);
int speedJa(char motorJa);
extern void calculateA(void);
extern void calculateB(void);
extern char unsigned newAValue(void);
extern char unsigned newBValue(void);
extern long unsigned gemmeValBYo(void);
extern long unsigned gemmeValAYo(void);