/*
 * TECLADO_PROJ1.c
 *
 * Created: 10/10/2025 16:13:15
 * Author : Aksa
 */

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#define F_CPU 16000000

#include <avr/io.h>

#include "protocolo.h"
#include "serial.h"
#include "teclado.h"

int main() {
  // para testes do teclado

  char flag = 0;
  DDRB = (1 << 7);
  teclado_init();
  while (1) {
    if (tecla() == '1') {
      if (flag == 0)
        PORTB ^= 1 << 7;
      flag = 1;
    } else {
      flag = 0;
    }
  }
  // fim do teste teclado
}
