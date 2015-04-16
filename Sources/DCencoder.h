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
#define LOOP_FLAG     0x02    //When another second is available

#define NUM_TRACKS    20

//Prototypes
void encoderInit(void);