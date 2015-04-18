/*************************************************************************
DCencoder.h

Author: Josiah Snarr

Date: April 15, 2015

DCencoder.h contains macros, constants, and prototypes for the
  clientProtocal.c functions
*************************************************************************/
#include "macros.h"

//Definitions
#define FIRST_CHAR    '<'
#define LAST_CHAR     '>'
#define SPACER        '\0'
#define PACKET_SIZE   20
#define PACKET_COUNT  5
#define BUFFER_SIZE   (PACKET_SIZE*PACKET_COUNT)

#define ERROR_CHECK   (SCI_FE && SCI_NF)

//Prototypes
int unsigned nbrPackets(void);
char * nextPacket(void);
void sendSinglePacket(char dataSend);
void sendPing(void);