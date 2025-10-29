#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer0_1.h"

void config_timer0(unsigned char p) {
  TCCR0A = 2;       // Modo com Comparacão
  TCCR0B = p;       // préscaler config
  TCNT0 = 0;        // Tempo de inicio
  TIFR0 = (1 << 1); // limpa flag
}

void Timer0_ms(unsigned char tempo_final) {
  config_timer0(4);
  OCR0A = tempo_final * 63;        // Tempo final da contagem   *62.5
  while ((TIFR0 & (1 << 1)) == 0); // Contagens até o flag "zerar"
  TIFR0 = (1 << 1);                // limpa flag
}

void config_timer1(unsigned char p) {
  TCCR1A = 0;       // Modo com Comparacão
  TCCR1B = p;       // préscaler config
  TCNT1 = 0;        // Tempo de inicio
  TIFR1 = (1 << 1); // limpa flag
}

void Timer1_us(unsigned char tempo_final) {
  config_timer1(10);
  OCR1A = tempo_final * 2;         // Tempo final da contagem   *2
  while ((TIFR1 & (1 << 1)) == 0); // Contagens até o flag "zerar"
  TIFR1 = (1 << 1);                // limpa flag
}

void Timer1_ms(unsigned char tempo_final) {
  config_timer1(12);
  OCR1A = tempo_final * 63;        // Tempo final da contagem   *62.5
  while ((TIFR1 & (1 << 1)) == 0); // Contagens até o flag "zerar"
  TIFR1 = (1 << 1);                // limpa flag
}
//
void Timer1(unsigned char tempo_final) {
  config_timer1(13);
  OCR1A = tempo_final * 15625;     // Tempo final da contagem *15625
  while ((TIFR1 & (1 << 1)) == 0); // Contagens até o flag "zerar"
  TIFR1 = (1 << 1);                // limpa flag
}
