#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

int BOUNCE = 6; // número de leituras estáveis para considerar a tecla pressionada

void atraso_debounce() {
  while ((TIFR0 & (1 << 0)) == 0); // enquanto a flag de interrupcao do timer 0 nao for 1
  TCNT0 = 0;
  TIFR0 |= (1 << 0); // zera a flag
}

// Inicializar teclado
void teclado_init() {
  TCCR0A = 0; // configuracao do timer zero para debounce
  TCCR0B = 4;
  TCNT0 = 0;

  DDRK &= ~(1 << 7 | 1 << 6 | 1 << 5 | 1 << 4); // define PORTK como metade entrada (colunas)
  DDRK |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);  // define PORTK como saida (linhas)
  PORTK |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);

  PINK |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3); // habilita pull-up nos pinos
}

// Funcao de debounce para leitura do teclado, a funcao recebe como argumento a coluna da matriz de contato
char le_coluna(char pino) {
  char count = 0, key_last = 0, key_now = 0;

  while (1) {
    atraso_debounce();

    key_now = PINK & (1 << (pino + 4));

    if (key_now == key_last) {
      count++;
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
    PORTK &= ~(1 << linha);

    // Itera as colunas do teclado
    for (coluna = 0; coluna < 3; coluna++) {
      if (!le_coluna(coluna)) {
        return teclado[linha][coluna];
      }
    }
  }
  // Caso nenhuma tecla seja pressionada, retorne 'f'
  return 'f';
}
