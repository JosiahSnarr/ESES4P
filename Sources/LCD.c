/*************************************************************************
LCD.c

Author: Josiah Snarr

Date: April 5, 2015

LCD.c contains the functions to control the LCD module
*************************************************************************/

#include "LCD_Macros.h"
#include "timer.h"
#include <stdio.h>

//Natesh's charachters
char upArrow[8] = 
{
	0b00000100,
	0b00001110,
	0b00011111,
	0b00000100,
	0b00000100,
	0b00000100,
	0b00000100,
	0b00000000
};

char heart[8] = 
{
	0b00000000,
	0b00001010,
	0b00011111,
	0b00011111,
	0b00011111,
	0b00001110,
	0b00000100,
	0b00000000
};

char smile[8] = 
{
	0b00000000,
	0b00000000,
	0b00001010,
	0b00000000,
	0b00010001,
	0b00001110,
	0b00000000,
	0b00000000
};

char person[8] = 
{
	0b00000100,
	0b00001010,
	0b00000100,
	0b00010101,
	0b00001110,
	0b00000100,
	0b00001010,
	0b00001010
};

//My characters
char motor[8] =
{
	0b00011111,
	0b00011111,
	0b00000100,
	0b00011111,
	0b00011111,
	0b00011111,
	0b00011111,
	0b00011111
};

char deathFace[8] =
{
	0b00010101,
	0b00001010,
	0b00010101,
	0b00000000,
	0b00000100,
	0b00001010,
	0b00001110,
	0b00010001
};

char robot[8] =
{
	0b00001110,
	0b00011111,
	0b00010101,
	0b00010101,
	0b00000100,
	0b00011111,
	0b00011011,
	0b00011011
};

char snowman[8] =
{
	0b00001110,
	0b00010101,
	0b00011111,
	0b00001110,
	0b00011111,
	0b00001110,
	0b00011111,
	0b00011111
};

//The LCDdata function writes a character to the data register of the LCD
void LCDdata(unsigned char thisByte)
{
	LCD_E_LO;
	
	LCD_RS_DR;
	
	LCD_RW_WRITE;
	
	LCD_E_HI;
	LCD_BUS(HI_NYBBLE(thisByte));
	LCD_E_LO;
	
	LCD_E_HI;
	LCD_BUS(LO_NYBBLE(thisByte));
	LCD_E_LO;
	
	msDelay(2);
}

void LCDcmd(unsigned char command)
{
  LCD_E_LO;
  
  LCD_RS_IN;
  
  LCD_RW_WRITE;
  
  LCD_E_HI;
  LCD_BUS(HI_NYBBLE(command));
  LCD_E_LO;
  
  LCD_E_HI;
  LCD_BUS(LO_NYBBLE(command));
  LCD_E_LO;
  
  msDelay(2);
}

void LCDclear(void)
{
  LCDcmd(LCD_CLR);
}

void LCDinit(void)
{
  //Initialize the port
  SET_BITS(LCD_PORT_DDR, LCD_DDR_INIT);
  msDelay(40);

	//Wake up display and sync
	LCD_E_HI;
	LCD_BUS(0x03);
	LCD_E_LO;
	
	msDelay(5);
	
	//Wake up display and sync
	LCD_E_HI;
	LCD_BUS(0x03);
	LCD_E_LO;
	
	msDelay(1);
	
	//Wake up display and sync
	LCD_E_HI;
	LCD_BUS(0x03);
	LCD_E_LO;
	
	//Wake up display and sync - got to 4
	LCD_E_HI;
	LCD_BUS(0x02);
	LCD_E_LO;
	
	msDelay(2);
	
	//Now configure display
	LCDcmd(LCD_CMD_FUNCTION|LCD_FUNCTION_4BIT);
	LCDcmd(LCD_CMD_FUNCTION|LCD_FUNCTION_2LINES|LCD_FUNCTION_5X8FONT);
	LCDcmd(LCD_CMD_DISPLAY|LCD_DISPLAY_OFF);
	LCDclear();
	LCDcmd(LCD_CMD_ENTRY|LCD_ENTRY_MOVE_CURSOR|LCD_ENTRY_INC);
	LCDcmd(LCD_CMD_DISPLAY|LCD_DISPLAY_ON|LCD_DISPLAY_NOCURSOR|LCD_DISPLAY_BLINK);
	
	msDelay(2);
	
	//My charachters
	LCDcgramInit(robot, 8, ROBOT);
	LCDcgramInit(snowman, 8, SNOWMAN);
	
	//Natesh's charachters
	LCDcgramInit(upArrow, 8, ARROW);
  LCDcgramInit(heart, 8, HEART);
  LCDcgramInit(smile, 8, SMILE);
  LCDcgramInit(person, 8, PERSON);
  
  //My charachters
  LCDcgramInit(motor, 8, MOTOR);
  LCDcgramInit(deathFace, 8, DEATH);
}

void LCDputc(char c)
{
  LCDdata(c);
}

void LCDputs(char *ptr)
{
  while(*ptr) //While still characters
  {
    switch(*ptr)
    {
      case NEW_LINE:
        LCDcmd(LCD_DDRAM_SET|LCD_DDRAM_LINE2);
        break;
        
      case HOME:
        LCDcmd(LCD_HOME);
        break;
        
      default:
        LCDputc(*ptr);  //print the character
        break;
    }
    ptr++;  //Go to next character
  }
}

void LCDprintf(char * format, ...)
{
  va_list args;
  
  char buffer[MAX_LCD_BUFSIZ];
  
  va_start(args, format);
  (void)vsprintf(buffer, format, args);
  va_end(args);
  
  LCDputs(buffer);
}

void LCDcgramInit(char * buffer, char numRow, char location)
{
  char count;
  
  for(count = 0; count < numRow; count++)
  {
    LCDcmd(LCD_CGRAM_SET|LCD_CGRAM_LOC(location)|LCD_CGRAM_ROW(count));
    LCDdata(buffer[count] & LCD_CGRAM_MASK);
  }
}