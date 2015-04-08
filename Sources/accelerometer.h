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

//Macros
#define ACC_SEL_X           CLR_BITS(ACC_FUNC_PORT, ACC_FUNC_BOTH_MASK);
#define ACC_SEL_Y           SET_BITS(ACC_FUNC_PORT, ACC_FUNC_PORT_S0);
#define ACC_SEL_Z           SET_BITS(ACC_FUNC_PORT, ACC_FUNC_PORT_S1);

#define CLR_ATDSTAT0_FLAG(x) (ATDSTAT0 = (1<<(x)))
#define CLR_ATDSTAT1_FLAG(x) (ATDSTAT1 = (1<<(x)))

//Prototypes
void accInit(void);
unsigned int getAccValue(unsigned char channel);
void setAccChnl(unsigned char channel);