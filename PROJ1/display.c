/*
 * display.c
 *
 * Created: 13/10/2025 13:48:50
 *  Author: Aksa
 */

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#define E 3
#define RS 2

#include <avr/io.h>
// display LCD 16x2

// Fun��o para escrever comandos no LCD. (no programa � usado para Function Set,
// Entry Mode Set, Display Control e Cursor Home e pular uma linha(0xC0) )
void escreve_comando_lcd(char comando) {
  PORTF &= ~(0xFF); // Zera todos os bits da porta F para limpar a mesma
  //_delay_us(1);
  PORTF &= ~(1 << RS); // Zera RS, para escrever um comando

  PORTF |= (1 << E); // Sobe E
  //_delay_us(1);
  PORTA = comando;    // Escreve o comando
  PORTF &= ~(1 << E); // Baixa E
  //_delay_us(40);
}

// Fun��o para limpar o display
void limpa_display() {
  escreve_comando_lcd(0x01); // Limpa a tela do display
  //_delay_ms(2);
  escreve_comando_lcd(0x80); // Retorna para a posi��o inicial
}

// Fun��o de Inicializa��o do Display LCD
void inicializa_display() {
  DDRA = 0xFF;
  DDRF = 0xFF; // Seta os pinos da porta A como sa�das, tendo em vista que os
               // pinos PF2 e PF3 ser�o respectivamente os sinais de sa�da RS e E

  //_delay_ms(25);

  escreve_comando_lcd(0x38); // Function Set - Configura o display para
                             // interface de 8 bits usando 2 linhas
  escreve_comando_lcd(0x0C); // Display Control - Display ON, cursor OFF
  escreve_comando_lcd(0x06); // Entry Mode Set - Incrementa o cursor automaticamente

  limpa_display(); // Fun��o para limpar o LCD
  //_delay_ms(2);				//Espera por 2 milisegundos
}

// Escrever/receber dados CHAR no LCD.(No programa ela recebe os caracteres das
// senhas e das mensagens char que queiramos exibir no display LDC)
void escreve_dado_char_lcd(char dado) {
  PORTF &= ~(0xFF); // Zera todos os bits da porta A para limpar a mesma
  //_delay_us(1);
  PORTF |= (1 << RS); // Seta RS em 1, para escrever dado

  PORTF |= (1 << E); // Sobe E
  //_delay_us(1);
  PORTA = dado;       // Escreve o dado char
  PORTF &= ~(1 << E); // Baixa E
  //_delay_us(40);
}

// Fun��o para escrever strings de char no display LCD. Enquanto a string n�o
// encontrar o caracter nulo, vai preenchendo a string com os bytes char
// recebidos da fun��o "escreve_dado_char_lcd"
void escreve_string_lcd(char *texto) {
  char i = 0;

  while (texto[i] != 0) {
    escreve_dado_char_lcd(texto[i]);
    i++;
  }
  //_delay_ms(200);
}
