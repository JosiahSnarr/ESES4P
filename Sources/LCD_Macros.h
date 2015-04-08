/*************************************************************************
LCD_Macros.h

Author: Josiah Snarr

Date: April 5, 2015

LCD_Macros.h contains the macros, constants and prototypes to support the
  LCD functions within LCD.c
*************************************************************************/
#include "macros.h"

//Special Characters
#define SNOWMAN       0
#define ROBOT         1
#define ARROW         2
#define HEART         3
#define SMILE         4
#define PERSON        5
#define MOTOR         6
#define DEATH         7

//Definitions
#define LCD_PORT		  PORTA
#define LCD_PORT_DDR	DDRA
#define LCD_DDR_INIT  0b11111111

#define LCD_E_MASK		PORTA_BIT4_MASK
#define LCD_RS_MASK		PORTA_BIT5_MASK
#define LCD_RW_MASK		PORTA_BIT6_MASK
#define LCD_BUS_MASK	0x0Fu

#define MAX_LCD_BUFSIZ  33

#define NEW_LINE      '\n'
#define HOME          '\r'

#define LCD_CMD_FUNCTION        0b00100000
#define LCD_FUNCTION_4BIT       0
#define LCD_FUNCTION_2LINES     0b00001000
#define LCD_FUNCTION_5X8FONT    0
#define LCD_CMD_DISPLAY         0b00001000
#define LCD_DISPLAY_OFF         0

#define LCD_CMD_ENTRY           0b00000100
#define LCD_ENTRY_MOVE_CURSOR   0b00010000
#define LCD_ENTRY_INC           0b00000100
#define LCD_ENTRY_DEC           0

#define LCD_DISPLAY_ON          0b00000100
#define LCD_DISPLAY_CURSOR      0b00000010
#define LCD_DISPLAY_NOCURSOR    0
#define LCD_DISPLAY_BLINK       0b00000001
#define LCD_DISPLAY_NOBLINK     0

#define LCD_CLR                 0b00000001
#define LCD_HOME                0b00000010
#define LCD_DDRAM_SET           0b10000000
#define LCD_DDRAM_LINE2         0b01000000

#define LCD_CGRAM_SET           0b01000000
#define LCD_CGRAM_LOC(loc)      (((loc) & 0b00000111) << 3)
#define LCD_CGRAM_ROW(row)      ((row) & 0b00000111)
#define LCD_CGRAM_MASK          0b00011111

//Macros
#define LCD_E_LO	CLR_BITS(LCD_PORT, LCD_E_MASK)
#define LCD_E_HI	SET_BITS(LCD_PORT, LCD_E_MASK)

#define LCD_RS_DR	SET_BITS(LCD_PORT, LCD_RS_MASK) //Data register
#define LCD_RS_IN	CLR_BITS(LCD_PORT, LCD_RS_MASK) //Instruction register

#define LCD_RW_WRITE	CLR_BITS(LCD_PORT, LCD_RW_MASK)
#define LCD_RW_READ		SET_BITS(LCD_PORT, LCD_RW_MASK)

#define LCD_BUS(value)	FORCE_BITS(LCD_PORT, LCD_BUS_MASK, (value))

//Prototypes
void LCDdata(unsigned char thisByte);
void LCDcmd(unsigned char command);
void LCDclear(void);
void LCDinit();
void putcSCI(char cx);
void putsSCI(char *ptr);
void LCDputc(char c);
void LCDputs(char *ptr);
void LCDprintf(char * format, ...);
void LCDcgramInit(char * buffer, char numRow, char location);