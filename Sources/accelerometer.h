/*************************************************************************
servo.h

Author: Josiah Snarr

Date: April 8, 2015

servo.h contains macros, constants, and prototypes for the accelerometer
  functions within accelerometer.c
*************************************************************************/
#include "macros.h"

//Definitions
#define ACC_FUNC_PORT       PTP
#define ACC_FUNC_DDR        DDRP
#define ACC_FUNC_PORT_S0    PTP_PTP3
#define ACC_FUNC_PORT_S1    PTP_PTP2
#define ACC_FUNC_BOTH_MASK  (PTP_PTP3_MASK|PTP_PTP2_MASK)

#define ATDCTL2_SETUP       0b11100000
#define ATDCTL3_SETUP       0b00001010
#define ATDCTL4_SETUP       0b00000011
#define ATDCTL5_SETUP       0b10000000

//Macros
#define ACC_SEL_X           CLR_BITS(ACC_FUNC_PORT, ACC_FUNC_BOTH_MASK);
#define ACC_SEL_Y           SET_BITS(ACC_FUNC_PORT, ACC_FUNC_PORT_S0);
#define ACC_SEL_Z           SET_BITS(ACC_FUNC_PORT, ACC_FUNC_PORT_S1);

//Prototypes
void accInit(void);
unsigned int getAccValue(unsigned char channel);
void setAccChnl(unsigned char channel);