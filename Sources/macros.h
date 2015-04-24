/*************************************************************************
macros.h

Author: Josiah Snarr

Date: March 20, 2015

macros.h contains macros to support the HCS12 embedded system project
*************************************************************************/

#include <hidef.h>
#include "derivative.h"

//Maxes & other common constants
#define MAX_CHAR    0xFF
#define MAX_INT     0xFFFF
#define MAX_LONG    0xFFFFFFFF

//Resgister
#define A_REGISTER  (*(volatile unsigned char * const) 0x1234)
#define TCTL_1_2    (*(volatile word * const)&TCTL1)
#define TCTL_3_4    (*(volatile word * const)&TCTL3)

//Standard macros from class
#define LOW(x) ((x) & 0xFF)
#define SET_BITS(P,V) ((P) |= (V))
#define CLR_BITS(P,V) ((P) &= LOW(~(V)))
#define FLIP_BITS(P,V) ((P) ^= (V))
#define FORCE_BITS(reg, mask, value) (reg)=((reg) & LOW(~(mask))|((value)&(mask)))
#define FORCE_WORD(reg, mask, value) (reg)=((reg) & ~(mask)|(value)&(mask))

#define ENABLE_PWM_CHNL(chnl) SET_BITS(PWME, 1<<(chnl))
#define DISABLE_PWM_CHNL(chnl) CLR_BITS(PWME, 1<<(chnl))

#define LO_NYBBLE(value)	((value)&0x0F)
#define HI_NYBBLE(value)	(((value)>>4)&0x0F)

#define IS_SET(x, y) ((x) & (y))
#define IS_CLR(x, y) (!((x) & (y)))

//Standard macros from Natesh
#define BV(b) (1 << (b))
#define SET_BIT(x, y) (SET_BITS((x), BV(y)))
#define CLR_BIT(x, y) (CLR_BITS((x), BV(y)))
#define TGL_BIT(x, y) (FLIP_BITS((x), BV(y)))

#define IS_BIT_SET(x, y) (IS_SET((x), BV(y)))
#define IS_BIT_CLR(x, y) (IS_CLR((x), BV(y)))

#define BIT_CAT(x, y, bits) (((x)<<(bits))+(y))

#define NYB_CAT(x, y) (BIT_CAT((x),(y),4))
#define BYTE_CAT(x,y) (BIT_CAT((x),(y),8))
#define WORD_CAT(x,y) (BIT_CAT((x),(y),16))

#define EXPAND_PORT(port) (*(volatile word * const) &port);

#define EVAL(x,y) x##y