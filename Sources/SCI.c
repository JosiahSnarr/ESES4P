/*************************************************************************
SCI.c

Author: Josiah Snarr

Date: April 9, 2015

SCI.c contains the functions to control the serial communcations interface
*************************************************************************/
#include "derivative.h"
#include "SCI.h"

static volatile char sciBuff[BOUND_BUFF_SIZE];
static volatile char inBuff;  //Buffer head
static volatile char outBuff; //Buffer tail

/*************************************************************************
Author: Josiah Snarr
Date: April 9, 2015

sciInit initializes the SCI
*************************************************************************/
void sciInit(void)
{
  SCIBD  = BAUD_INIT;
  SCICR1 = SCICR1_INIT;
  SCICR2 = SCICR2_INIT;
  inBuff = 0;
  outBuff = 50;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 10, 2015

sciDequeue returns the next charachter in the sci buffer
*************************************************************************/
char sciDequeue(void)
{
  char nChar;
  
  DisableInterrupts;
  if(inBuff != outBuff)   //If tail != head, get the value
  {
    outBuff = (char)((outBuff+1) % BOUND_BUFF_SIZE);
    nChar = sciBuff[outBuff];
  }
  else                    //Otherwise return an no-charachter notice
  {
    nChar = NO_CHAR;
  }
  EnableInterrupts;
  
  return(nChar);
}

/*************************************************************************
Author: Josiah Snarr
Date: April 9, 2015

putcSCI pushes a byte-long value through the SCI
*************************************************************************/
void putcSCI(unsigned char cx)
{
  while(!(SCISR1 & SCISR1_TDRE_MASK));
  SCIDRL = cx;
}

/*************************************************************************
Author: Josiah Snarr
Date: April 9, 2015

putsSCI pushes a string through the SCI
*************************************************************************/
void putsSCI(unsigned char *ptr)
{
  while(*ptr) //While still characters to output
  {
    putcSCI(*ptr++);
  }
}

/*************************************************************************
Author: Josiah Snarr
Date: April 10, 2015

the sciReceive interrupt handles incommign data and places it into the
  bound buffer
*************************************************************************/
interrupt VectorNumber_Vsci void sciReceive(void)
{
  sciBuff[inBuff] = SCIDRL;
  inBuff = (char)((inBuff+1) % BOUND_BUFF_SIZE);
  
  if(inBuff == outBuff) //Check if buffer overwrote, if so increment tail
  {
    outBuff = (char)((outBuff+1) % BOUND_BUFF_SIZE);
  }
  CLR_BITS(SCISR1, SCISR1_RDRF_MASK);
}