/*************************************************************************
SCI.c

Author: Josiah Snarr

Date: April 9, 2015

SCI.c contains the functions to control the serial communcations interface
*************************************************************************/
#include "derivative.h"
#include "SCI.h"

void sciInit(void)
{
  SCIBD = BAUD_INIT;
  SCICR1 = SCICR1_INIT;
  SCICR2 = SCICR2_INIT;
}

interrupt VectorNumber_Vsci void sciReceive(void)
{
  
}