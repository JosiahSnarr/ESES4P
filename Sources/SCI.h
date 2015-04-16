/*************************************************************************
SCI.h

Author: Josiah Snarr

Date: April 9, 2015

SCI.h contains macros, constants, and prototypes for the serial interface
  functions within SCI.c
*************************************************************************/
#include "macros.h"

//Definitions
#define BAUD_INIT     (8000000/16/9600)    //9600 baud

#define SCICR1_INIT   0           //0 for "normal" N81 operation
#define SCICR2_INIT   0b00101100  //Turn on receiver and transmitter, receiver interrupt enabled

#define BOUND_BUFF_SIZE   50      //Up to 50 elements in the bounded buffer

#define NO_CHAR       127         //No charachter value

//Prototypes
extern void sciInit(void);
extern char sciDequeue(void);
extern void putcSCI(unsigned char cx);
extern void putsSCI(unsigned char *ptr);