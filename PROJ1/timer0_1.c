#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer0_1.h"

void config_timer0(unsigned char p) {
  TCCR0A = 2;       // Modo com Comparacao
  TCCR0B = p;       // prescaler config
  TCNT0 = 0;        // Tempo de inicio
  TIFR0 = (1 << 1); // limpa flag
}

void Timer0_ms(unsigned char tempo_final) {
  config_timer0(4);
  OCR0A = tempo_final * 63;        // Tempo final da contagem   *62.5
  while ((TIFR0 & (1 << 1)) == 0); // Contagens ate o flag "zerar"
  TIFR0 = (1 << 1);
}
// funcao que configura timer 1
void config_timer1(unsigned char p) {
  TCCR1A = 0;
  TCCR1B = p;
  TCNT1 = 0;
  TIFR1 = (1 << 1);
}
// as veriacoes da funcao timer1 para s, ms e us
void Timer1_us(unsigned char tempo_final) {
  config_timer1(10);
  OCR1A = tempo_final * 2; // Tempo final da contagem   *2
  while ((TIFR1 & (1 << 1)) == 0);
  TIFR1 = (1 << 1);
}

void Timer1_ms(unsigned char tempo_final) {
  config_timer1(12);
  OCR1A = tempo_final * 63; // Tempo final da contagem   *62.5
  while ((TIFR1 & (1 << 1)) == 0);
  TIFR1 = (1 << 1);
}
//
void Timer1(unsigned char tempo_final) {
  config_timer1(13);
  OCR1A = tempo_final * 15625; // Tempo final da contagem *15625
  while ((TIFR1 & (1 << 1)) == 0);
  TIFR1 = (1 << 1);
}
