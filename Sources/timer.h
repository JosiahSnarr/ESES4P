/*************************************************************************
timer.h

Author: Josiah Snarr

Date: March 28, 2015

timer.h contains macros, constants, and prototypes for the timer.c
  functions
*************************************************************************/
#include "macros.h"
#include "derivative.h"

/* Timer Modules Prescalers */

#define TIMER_PRESCALER_1    0x00
#define TIMER_PRESCALER_2    0x01
#define TIMER_PRESCALER_4    0x02
#define TIMER_PRESCALER_8    0x03
#define TIMER_PRESCALER_16   0x04
#define TIMER_PRESCALER_32   0x05
#define TIMER_PRESCALER_64   0x06
#define TIMER_PRESCALER_128  0x07

#define TIMER_PRESCALER_MASK 0x07

#define TIMER_SET_PRESCALER(prescale) \
  CLR_BITS(TSCR2, TIMER_PRESCALER_MASK); \
  SET_BITS(TSCR2, prescale)

/* Timer System Control */

#define TIMER_ENABLE()              SET_BITS(TSCR1, TSCR1_TEN_MASK)
#define TIMER_DISABLE()             CLR_BITS(TSCR1, TSCR1_TEN_MASK)

#define TIMER_DISABLE_ON_FREEZE()   SET_BITS(TSCR1, TSCR1_TSFRZ_MASK)
#define TIMER_ENABLE_ON_FREEZE()    CLR_BITS(TSCR1, TSCR1_TSFRZ_MASK)

#define TIMER_NORMAL_FAST_CLEAR()   CLR_BITS(TSCR1, TSCR1_TFFCA_MASK)
#define TIMER_FANCY_FAST_CLEAR()    SET_BITS(TSCR1, TSCR1_TFFCA_MASK)

/* Input Capture / Output Compare */

#define MAKE_CHNL_OC(x) (TIOS |= (x))
#define MAKE_CHNL_IC(x) (TIOS &= (~(x)))

#define TIMER_CHNL_ENABLE_INT(chnl) SET_BIT(TIE, chnl)

#define CLR_TIMER_CHNL_FLAG(x) (TFLG1 =(1<<(x)))

/* Timer Flag Register */

#define TIMER_CHNL_EVENT(chnl)      IS_BIT_SET(TFLG1, chnl)

/* Output Compare Actions */

#define TIMER_OC_ACTION_NO_ACTIONS 0x00
#define TIMER_OC_ACTION_TOGGLE_PIN 0x01
#define TIMER_OC_ACTION_DRIVE_LOW  0x02
#define TIMER_OC_ACTION_DRIVE_HIGH 0x03

#define TIMER_IC_ACTION_NO_ACTION          0x00
#define TIMER_IC_ACTION_RISING_EDGE        0x01
#define TIMER_IC_ACTION_FALLING_EDGE       0x02
#define TIMER_IC_ACTION_RISING_AND_FALLING 0x03

#define SET_OC_ACTION(chnl, action) FORCE_WORD(TCTL_1_2, 0x03<<((chnl)*2), (action)<<((chnl)*2))
#define SET_IC_ACTION(chnl, action) FORCE_WORD(TCTL_3_4, 0x03<<((chnl)*2), (action)<<((chnl)*2))

#define FORCE_OC_ACTION_NOW(chnl, action) SET_OC_ACTION((chnl), (action)); SET_BITS(CFORC, 1<<(chnl))
#define FORCE_IC_ACTION_NOW(chnl, action) SET_IC_ACTION((chnl), (action)); SET_BITS(CFORC, 1<<(chnl))

#define TCHNL(chnl) EVAL(TC, chnl)

/* Named Timer Channels */

#define TIMER_CHNL_DELAY 0

/* Prototypes */

void timer_init(void);

void msDelay(int waitTime);




