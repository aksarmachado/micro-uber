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

volatile uint32_t overflow_count = 0;

void timer1_init() {
  TCCR1A = 0x00;         // Configura o Timer1 em modo normal
  TCCR1B = (1 << CS12);  // Prescaler = 256
  TCNT1 = 0;             // Inicializa counter
  TIMSK1 = (1 << TOIE1); // Habilita interrupção por overflow do Timer1
  sei();                 // Habilita interrupções globais
}

ISR(TIMER1_OVF_vect) {
  overflow_count++; // Incrementa overflow count
}

uint32_t get_elapsed_time_ms() {
  uint32_t total_ticks;
  uint32_t elapsed_time_ms;

  // Desabilita interrupção para leitura consistente de overflow_count e TCNT1
  uint8_t sreg = SREG;
  cli();
  total_ticks = (overflow_count * 65536UL) + TCNT1;
  SREG = sreg;

  // Converte para milissegundos
  elapsed_time_ms = (total_ticks * 1000UL * 256UL) / F_CPU;

  return elapsed_time_ms;
}
