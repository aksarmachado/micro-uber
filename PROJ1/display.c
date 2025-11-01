#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include "timer0_1.h"
#include <avr/io.h>

#include <avr/interrupt.h>

#define LCD_Data_Dir DDRA    // Definicao das portas (8 bits)
#define LCD_Command_Dir DDRF // Definicao das portas para comando (EN, RS, RW) e backlight
#define LCD_Data_Port PORTA
#define LCD_Command_Port PORTF
#define LCD_Backlight_Dir DDRF
#define LCD_Backlight_Port PORTF
#define LCD_Backlight_Pin PF4
#define RS PF2
#define RW PF1
#define EN PF3

// Envia comando para o LCD
void LCD_Command(unsigned char cmnd) {
  LCD_Data_Port = cmnd;
  LCD_Command_Port &= ~(1 << RS);
  LCD_Command_Port &= ~(1 << RW);
  LCD_Command_Port |= (1 << EN);
  Timer1_us(1);
  LCD_Command_Port &= ~(1 << EN);
  Timer1_ms(3);
}

// Escreve caracteres
void LCD_Char(unsigned char char_data) {
  LCD_Data_Port = char_data;
  LCD_Command_Port |= (1 << RS);  // RS = 1
  LCD_Command_Port &= ~(1 << RW); // RW = 0, escrita
  LCD_Command_Port |= (1 << EN);  // Habilita Enable
  Timer1_us(1);
  LCD_Command_Port &= ~(1 << EN); // Desabilita Enable
  Timer1_ms(1);
}

// Inicializa display
void LCD_Init(void) {
  LCD_Command_Dir = 0xFF; // Definindo as portas como saida
  LCD_Data_Dir = 0xFF;
  LCD_Backlight_Dir |= (1 << LCD_Backlight_Pin);  // Define como saida
  LCD_Backlight_Port |= (1 << LCD_Backlight_Pin); // Liga o backlight

  Timer1_ms(20);
  LCD_Command(0x38); // Inicializa no modo 8bits
  LCD_Command(0x0C); // Cursor ON/OFF
  LCD_Command(0x06); // Habilita autoincremento
  LCD_Command(0x01); // Limpa o display
  LCD_Command(0x80); // Seta cursor para posicao (0, 0)
}

// Desliga display
void LCD_Off() {
  LCD_Backlight_Port &= ~(1 << LCD_Backlight_Pin); // Desliga o backlight
  LCD_Command(0x08);                               // Display OFF (sem apagar dados)
}

// Escreve string no display
void LCD_String(char *str) {
  int i;
  for (i = 0; str[i] != 0; i++) // Envia sequencia de caracteres
  {
    LCD_Char(str[i]);
  }
}

// Envia uma string para uma posicao especifica
void LCD_String_xy(char row, char pos, char *str) {
  if (row == 0 && pos < 16)
    LCD_Command((pos & 0x0F) | 0x80); // se linha 0, soma a posicao (pos) ao valor 0, sem alterar os primeiros 4 bits
  else if (row == 1 && pos < 16)
    LCD_Command((pos & 0x0F) | 0xC0);
  LCD_String(str);
}

// Limpa display
void LCD_Clear() {
  LCD_Command(0x01);
  LCD_Command(0x80);
}