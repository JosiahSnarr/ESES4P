/*************************************************************************
SCI.h

Author: Josiah Snarr

Date: April 9, 2015

SCI.h contains macros, constants, and prototypes for the serial interface
  functions within SCI.c
*************************************************************************/
#include "macros.h"

//Definitions
#define BAUD_INIT     17          //17 for ~57600 baud (16M/16/57600)
#define SCICR1_INIT   0           //0 for "normal" N81 operation
#define SCICR2_INIT   0b00101100  //Turn on receiver and transmitter, receiver ointerrupt enabled
#define 

//Prototypes
void sciInit(void);