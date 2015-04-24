/*************************************************************************
DCmotor.h

Author: Josiah Snarr

Date: April 11, 2015

DCmotor.h contains macros, constants, and prototypes for the DC motor
  functions in DCmotor.c
*************************************************************************/
#include "macros.h"

//Definitions
#define DC_PORT         PORTB
#define DC_DDR          DDRB
#define MOTOR_SEL_MASK  0x0F
#define MOTOR_B_1       0x02
#define MOTOR_B_2       0x08
#define MOTOR_A_1       0x01
#define MOTOR_A_2       0x04

#define MOTORS_MASK     0x30    //0b00110000

#define CLOCK_SCALE     0x06    //Prescaler of 64 (125kHz)

#define PWM_PER         0x3F    //1.024ms period

#define MAX_DUTY        0xFF    //Max on duty corresponds to no off duty cycle  (motors max speed)
#define MIN_DUTY        0x00    //Min on duty corresponds to max off duty cycle (motors off)

#define DIREC_FOR       0
#define DIREC_BAC       1

#define START_DUTY      20      //Starting duty cycle

//Macros
#define ENABLE_MOTOR(mask)    SET_BITS(PWME, mask)         //output pwm to motors
#define ENABLE_A              ENABLE_MOTOR((BV(4)))
#define ENABLE_B              ENABLE_MOTOR((BV(5)))
#define ENABLE_BOTH           ENABLE_MOTOR(MOTORS_MASK)

#define DISABLE_MOTOR(mask)   CLR_BITS(PWME, mask)
#define DISABLE_A             DISABLE_MOTOR((BV(4)))
#define DISABLE_B             DISABLE_MOTOR((BV(5)))
#define DISABLE_BOTH          DISABLE_MOTOR(MOTORS_MASK)

#define DUTY_A(duty)          PWMDTY4 = (duty)
#define DUTY_B(duty)          PWMDTY5 = (duty)
#define DUTY_BOTH(duty)       DUTY_A(duty);DUTY_B(duty)

#define FORWARD_A             SET_BITS(DC_PORT, MOTOR_A_1);CLR_BITS(DC_PORT, MOTOR_A_2)
#define REVERSE_A             SET_BITS(DC_PORT, MOTOR_A_2);CLR_BITS(DC_PORT, MOTOR_A_1)
#define FORWARD_B             SET_BITS(DC_PORT, MOTOR_B_1);CLR_BITS(DC_PORT, MOTOR_B_2)
#define REVERSE_B             SET_BITS(DC_PORT, MOTOR_B_2);CLR_BITS(DC_PORT, MOTOR_B_1)
#define FORWARD_BOTH          SET_BITS(DC_PORT, MOTOR_A_1|MOTOR_B_1);CLR_BITS(DC_PORT, MOTOR_A_2|MOTOR_B_2)
#define REVERSE_BOTH          SET_BITS(DC_PORT, MOTOR_A_2|MOTOR_B_2);CLR_BITS(DC_PORT, MOTOR_A_1|MOTOR_B_1)

//Prototypes
extern void DCinit(void);
extern void DCstart(void);
extern void DCstop(void);
extern void setDuty(char unsigned amount);
extern void DCdirec(char mot, char direc);