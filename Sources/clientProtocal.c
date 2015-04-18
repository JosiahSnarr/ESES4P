/*************************************************************************
DCencoder.c

Author: Josiah Snarr

Date: April 17, 2015

clientProtocal.c contains the functions to control the client handling of
  incoming commands through RS232
*************************************************************************/
#include "derivative.h"
#include "clientProtocal.h"
#include "SCI.h"

char static packBuff[BUFFER_SIZE];
int static  index = 0;
int static unsigned numPacket = 0;          //Number of packets
int static unsigned packOff[PACKET_COUNT];  //Packet offset

//Get number of packets left
int unsigned nbrPackets(void)
{
  return numPacket;
}

//Get next packet
char * nextPacket(void)
{
  return packBuff + packOff[0];
}

/*************************************************************************
Author: Josiah Snarr
Date: April 18, 2015

the sendPacket function sends a packet through RS232 to a host
*************************************************************************/
void sendSinglePacket(char dataSend)
{
  putcSCI('<');
  putcSCI(dataSend);
  putcSCI('>');
}

/*************************************************************************
Author: Josiah Snarr
Date: April 18, 2015

the sendPing function sends a ping through RS232 to a host
*************************************************************************/
void sendPing(void)
{
  sendSinglePacket('P');
}

/*************************************************************************
Author: Josiah Snarr
Date: April 18, 2015

the sciReceive interrupt handles incomming data and places it into the
  bound buffer
*************************************************************************/
interrupt VectorNumber_Vsci void sciReceive(void)
{
  char static unsigned started = 0;
  char const newData = SCIDRL;
  
  if(IS_SET(SCISR1, SCISR1_RDRF_MASK))    //If received data
  {
    if(newData == FIRST_CHAR)   //Check if start of packet
    {
      packBuff[index] = newData;
      started++;
      packOff[numPacket] = index++;
    }
    else if(newData == LAST_CHAR)
    {
      if(started > 0) //Otherwise garbage
      {
        //Full packet
        packBuff[index] = newData;
        numPacket++;
        started = 0;
        
        //Add charachter to separate packet
        packBuff[++index] = SPACER;
        index++;
      }
    }
    else  //Non-edge of packet
    {
      if(started > 0) //Otherwise garbage
      {
        packBuff[index++] = newData;
      }
    }
  }
}