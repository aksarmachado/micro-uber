/*
 * timer.c
 *
 * Created: 18/10/2025 19:01:59
 *  Author: Aksa
 */

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#define F_CPU 16000000

#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer.h"

volatile float overflow_count = 0;

void timer2_init() {
  TCCR2A = 0x00;         // Configura o Timer1 em modo normal
  TCCR2B = (1 << CS12);  // Prescaler = 256
  TCNT2 = 0;             // Inicializa counter
  TIMSK2 = (1 << TOIE2); // Habilita interrupção por overflow do Timer1
  sei();                 // Habilita interrupções globais
}

ISR(TIMER2_OVF_vect) {
  overflow_count++; // Incrementa overflow count
}

float get_elapsed_time_ms() {
  float total_ticks;
  float elapsed_time_ms;

  // Desabilita interrupção para leitura consistente de overflow_count e TCNT1
  int sreg = SREG;
  cli();
  total_ticks = (overflow_count * 65536UL) + TCNT1;
  SREG = sreg;

  // Converte para milissegundos
  elapsed_time_ms = (total_ticks * 1000UL * 256UL) / F_CPU;

  return elapsed_time_ms;
}

// GABRIEL

// #include <avr/io.h>

// #include "Timer.h"

// Freq. UNO = 16MHz

//***TIMER 0***//

// Timer0_ms conta até 4 ms

// Préscaler config Timer 1:
// p=2, préscaler = 8
// p=3, préscaler = 64
// p=4, préscaler = 256
// p=5, préscaler = 1024

//***TIMER 1***//

// Timer1_us conta até 32767 us
// Timer1_ms conta até 1040 ms
// Timer1 conta até 4 s

// Préscaler config Timer 1:
// p=10, préscaler = 8
// p=11, préscaler = 64
// p=12, préscaler = 256
// p=13, préscaler = 1024

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
