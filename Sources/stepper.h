/*************************************************************************
stepper.h

Author: Josiah Snarr

Date: April 11, 2015

stepper.h contains macros, constants, and prototypes for the stepper motor
  functions within stepper.c
*************************************************************************/
#include "macros.h"

//Definitions
#define STEP_NUM_POS    8

#define STEP_PORT       PTT
#define STEP_DDR        DDRT
#define STEP_OUT        0xF0

#define STEP_SWI_PORT   PTAD
#define STEP_SWI_DDR    DDRAD
#define STEP_SWI_DIS    ATDDIEN   //Switch direction set (register)
#define STEP_SWI_IN     0xC0
#define SWI_RIGHT       PTT_PTT7_MASK
#define SWI_LEFT        PTT_PTT6_MASK

#define DIR_RIGHT       1         //Stepper direction right
#define DIR_LEFT        -1        //Stepper direction left

#define INDEX_MASK      0x07      //Mask for index into table (only 0-7)

//Macros
#define SWI_DIR(x)      ((x)*=-1)   //Switch direction (toggle)

//Prototypes
void stepperInit(void);
void stepperStep(void);