/*
 * display.c
 *
 * Created: 13/10/2025 13:48:50
 *  Author: Aksa
 */

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

// #define E 3
// #define RS 2
// #define F_CPU 16000000

// #include <avr/io.h>
// #include <util/delay.h>

// // display LCD 16x2

// // Fun��o para escrever comandos no LCD. (no programa � usado para Function Set,
// // Entry Mode Set, Display Control e Cursor Home e pular uma linha(0xC0) )
// void escreve_comando_lcd(char comando) {
//   PORTF &= ~(0xFF); // Zera todos os bits da porta F para limpar a mesma
//   _delay_us(1);
//   PORTF &= ~(1 << RS); // Zera RS, para escrever um comando

//   PORTF |= (1 << E); // Sobe E
//   _delay_us(1);
//   PORTA = comando;    // Escreve o comando
//   PORTF &= ~(1 << E); // Baixa E
//   _delay_us(40);
// }

// // Fun��o para limpar o display
// void limpa_display() {
//   escreve_comando_lcd(0x01); // Limpa a tela do display
//  _delay_ms(2);
//   escreve_comando_lcd(0x80); // Retorna para a posi��o inicial
// }

// // Fun��o de Inicializa��o do Display LCD
// void inicializa_display() {
//   DDRA = 0xFF;
//   DDRF = 0xFF; // Seta os pinos da porta A como sa�das, tendo em vista que os
//                // pinos PF2 e PF3 ser�o respectivamente os sinais de sa�da RS e E

//  _delay_ms(25);

//   escreve_comando_lcd(0x38); // Function Set - Configura o display para
//                              // interface de 8 bits usando 2 linhas
//   escreve_comando_lcd(0x0C); // Display Control - Display ON, cursor OFF
//   escreve_comando_lcd(0x06); // Entry Mode Set - Incrementa o cursor automaticamente

//   limpa_display(); // Fun��o para limpar o LCD
//   _delay_ms(2);				//Espera por 2 milisegundos
// }

// // Escrever/receber dados CHAR no LCD.(No programa ela recebe os caracteres das
// // senhas e das mensagens char que queiramos exibir no display LDC)
// void escreve_dado_char_lcd(char dado) {
//   PORTF &= ~(0xFF); // Zera todos os bits da porta A para limpar a mesma
//   _delay_us(1);
//   PORTF |= (1 << RS); // Seta RS em 1, para escrever dado

//   PORTF |= (1 << E); // Sobe E
//   _delay_us(1);
//   PORTA = dado;       // Escreve o dado char
//   PORTF &= ~(1 << E); // Baixa E
//   _delay_us(40);
// }

// // Fun��o para escrever strings de char no display LCD. Enquanto a string n�o
// // encontrar o caracter nulo, vai preenchendo a string com os bytes char
// // recebidos da fun��o "escreve_dado_char_lcd"
// void escreve_string_lcd(char *texto) {
//   char i = 0;

//   while (texto[i] != 0) {
//     escreve_dado_char_lcd(texto[i]);
//     i++;
//   }
//   _delay_ms(200);
// }

#include "timer.h"

// #define F_CPU 16000000UL			/* Define CPU Frequency e.g. here 8MHz */
#include <avr/io.h> /* Include AVR std. library file */
// #include <util/delay.h>			/* Include inbuilt defined Delay header file */

#define LCD_Data_Dir DDRA      /* Define LCD data port direction */
#define LCD_Command_Dir DDRF   /* Define LCD command port direction register */
#define LCD_Data_Port PORTA    /* Define LCD data port */
#define LCD_Command_Port PORTF /* Define LCD data port */
#define RS PF2                 /* Define Register Select (data/command reg.)pin */
#define RW PF1                 /* Define Read/Write signal pin */
#define EN PF3                 /* Define Enable signal pin */

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
  LCD_Command_Dir = 0xFF; /* Make LCD command port direction as o/p */
  LCD_Data_Dir = 0xFF;    /* Make LCD data port direction as o/p */
  //_delay_ms(20);			/* LCD Power ON delay always >15ms */
  Timer1_ms(20);

  LCD_Command(0x38); /* Initialization of 16X2 LCD in 8bit mode */
  LCD_Command(0x0C); /* Display ON Cursor OFF */
  LCD_Command(0x06); /* Auto Increment cursor */
  LCD_Command(0x01); /* Clear display */
  LCD_Command(0x80); /* Cursor at home position */
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