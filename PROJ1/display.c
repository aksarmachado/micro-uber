#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include "timer0_1.h"

#include <avr/io.h> /* Include AVR std. library file */

#define LCD_Data_Dir DDRA        /* Define LCD data port direction */
#define LCD_Command_Dir DDRF     /* Define LCD command port direction register */
#define LCD_Data_Port PORTA      /* Define LCD data port */
#define LCD_Command_Port PORTF   /* Define LCD data port */
#define LCD_Backlight_Dir DDRF   /* Define LCD backlight port direction */
#define LCD_Backlight_Port PORTF /* Define LCD backlight port */
#define LCD_Backlight_Pin PF4    /* Define LCD backlight pin */
#define RS PF2                   /* Define Register Select (data/command reg.)pin */
#define RW PF1                   /* Define Read/Write signal pin */
#define EN PF3                   /* Define Enable signal pin */

#include <avr/interrupt.h>

void LCD_Command(unsigned char cmnd) {
  LCD_Data_Port = cmnd;
  LCD_Command_Port &= ~(1 << RS); /* RS=0 command reg. */
  LCD_Command_Port &= ~(1 << RW); /* RW=0 Write operation */
  LCD_Command_Port |= (1 << EN);  /* Enable pulse */
  //_delay_us(1);
  Timer1_us(1);
  LCD_Command_Port &= ~(1 << EN);
  //_delay_ms(3);
  Timer1_ms(3);
}

void LCD_Char(unsigned char char_data) /* LCD data write function */
{
  LCD_Data_Port = char_data;
  LCD_Command_Port |= (1 << RS);  /* RS=1 Data reg. */
  LCD_Command_Port &= ~(1 << RW); /* RW=0 write operation */
  LCD_Command_Port |= (1 << EN);  /* Enable Pulse */
  //_delay_us(1);
  Timer1_us(1);
  LCD_Command_Port &= ~(1 << EN);
  //_delay_ms(1);
  Timer1_ms(1);
}

void LCD_Init(void) /* LCD Initialize function */
{
  LCD_Command_Dir = 0xFF;                         /* Make LCD command port direction as o/p */
  LCD_Data_Dir = 0xFF;                            /* Make LCD data port direction as o/p */
  LCD_Backlight_Dir |= (1 << LCD_Backlight_Pin);  // Define como saída
  LCD_Backlight_Port |= (1 << LCD_Backlight_Pin); // Liga o backlight

  Timer1_ms(20);
  LCD_Command(0x38); /* Initialization of 16X2 LCD in 8bit mode */
  LCD_Command(0x0C); /* Display ON Cursor OFF */
  LCD_Command(0x06); /* Auto Increment cursor */
  LCD_Command(0x01); /* Clear display */
  LCD_Command(0x80); /* Cursor at home position */
}

// Turn off lighting of LCD
void LCD_Off() {
  LCD_Backlight_Port &= ~(1 << LCD_Backlight_Pin); // Desliga o backlight
  LCD_Command(0x08);                               // Opcional: Display OFF (sem apagar dados)
}

void LCD_String(char *str) /* Send string to LCD function */
{
  int i;
  for (i = 0; str[i] != 0; i++) /* Send each char of string till the NULL */
  {
    LCD_Char(str[i]);
  }
}

void LCD_String_xy(char row, char pos, char *str) /* Send string to LCD with xy position */
{
  if (row == 0 && pos < 16)
    LCD_Command((pos & 0x0F) | 0x80); /* Command of first row and required position<16 */
  else if (row == 1 && pos < 16)
    LCD_Command((pos & 0x0F) | 0xC0); /* Command of first row and required position<16 */
  LCD_String(str);                    /* Call LCD string function */
}

void LCD_Clear() {
  LCD_Command(0x01); /* clear display */
  LCD_Command(0x80); /* cursor at home position */
}