#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#define F_CPU 16000000

#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer2.h"

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
