#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer2.h"

volatile unsigned long overflow_count = 0;

void timer2_init() {
  TCCR2A = 0x00;                      // Modo normal
  TCCR2B = (1 << CS22) | (1 << CS21); // Prescaler = 256
  TCNT2 = 0;                          // Zera contador
  TIMSK2 = (1 << TOIE2);              // Habilita interrupção de overflow
  sei();                              // Habilita interrupções globais
}

ISR(TIMER2_OVF_vect) { overflow_count++; }

float get_elapsed_time_ms() {
  unsigned long total_ticks;
  float elapsed_time_ms;

  // Protege leitura
  unsigned int sreg = SREG;
  cli();
  total_ticks = (overflow_count * 256UL) + TCNT2;
  SREG = sreg;

  // Cada tick = (prescaler / F_CPU) segundos
  // => tempo(ms) = ticks * prescaler * 1000 / F_CPU
  elapsed_time_ms = (total_ticks * 256.0 * 1000.0) / F_CPU;

  return elapsed_time_ms;
}
