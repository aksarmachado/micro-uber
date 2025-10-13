#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

int BOUNCE = 8;

void atraso_debounce() {
  while ((TIFR0 & (1 << 0)) == 0); // enquanto a flag de interrup��o do timer 0 n�o for 1
  TCNT0 = 31;
  TIFR0 |= (1 << 0); // zera a flag
}

// Inicializar teclado
void teclado_init() {
  TCCR0A = 0; // configur���o do timer zero para debounce
  TCCR0B = 4;
  TCNT0 = 31;

  DDRK &= ~(1 << 7 | 1 << 6 | 1 << 5 | 1 << 4); // define PORTK como metade entrada (colunas)
  DDRK |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);  // define PORTK como sa�da (linhas)
  PORTK |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);

  PINK |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3); // habilita pull-up nos pinos
}

// Fun��o de debounce para leitura do teclado, a fun��o recebe como argumento a
// coluna da matriz de contato
char le_coluna(char pino) {
  char count = 0, key_last = 0, key_now = 0;

  while (1) {
    atraso_debounce();

    key_now = PINK & (1 << (pino + 4));

    if (key_now == key_last) {
      count = count + 1;
    } else {
      count = 0;
    }

    if (count == BOUNCE) {
      if (key_now)
        return 1;
      else
        return 0;
    } else {
      key_last = key_now;
    }
  }
}

// Retorna tecla pressionada
char tecla() {
  int linha = 0, coluna = 0;

  char teclado[4][4] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};

  for (linha = 0; linha < 4; linha++) {
    PORTK |= 0x0F;
    PORTK &= ~(1 << linha); // Itera as colunas do teclado

    for (coluna = 0; coluna < 3; coluna++) {
      if (!le_coluna(coluna)) {
        // while(!le_coluna(coluna));
        return teclado[linha][coluna];
      }
    }
  }
  return 0; // caso nenhuma tecla seja pressionada
}
