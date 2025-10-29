#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer0_1.h"

#define F_CPU 16000000UL

//***TIMER 0***//

// Timer0_ms conta at� 4 ms

// Pr�scaler config Timer 1:
// p=4, pr�scaler = 256

//***TIMER 1***//

// Timer1_us conta at� 32767 us
// Timer1_ms conta at� 1040 ms
// Timer1 conta at� 4 s

// Pr�scaler config Timer 1:
// p=10, pr�scaler = 8
// p=12, pr�scaler = 256
// p=13, pr�scaler = 1024

// void config_timer0(unsigned char p) {
//   TCCR0A = 2;       // Modo com Comparacão
//   TCCR0B = p;       // préscaler config
//   TCNT0 = 0;        // Tempo de inicio
//   TIFR0 = (1 << 1); // limpa flag
// }

// void Timer0_ms(unsigned char tempo_final) {
//   config_timer0(4);
//   OCR0A = tempo_final * 63;        // Tempo final da contagem   *62.5
//   while ((TIFR0 & (1 << 1)) == 0); // Contagens até o flag "zerar"
//   TIFR0 = (1 << 1);                // limpa flag
// }

void config_timer1(unsigned int prescaler) {
  TCCR1A = 0; // Modo com Comparacão

  switch (prescaler) {
  case 1:
    TCCR1B |= (1 << CS10);
    break;
  case 8:
    TCCR1B |= (1 << CS11);
    break;
  case 64:
    TCCR1B |= (1 << CS11) | (1 << CS10);
    break;
  case 256:
    TCCR1B |= (1 << CS12);
    break;
  case 1024:
    TCCR1B |= (1 << CS12) | (1 << CS10);
    break;
  default:
    break;
  }

  TCCR1B |= (1 << WGM12); // modo CTC (limpa no compare)
  TIFR1 = (1 << OCF1A);   // limpa flag
  TCNT1 = 0;              // Zera o contador para iniciar a contagem a partir de 0
}

void Timer1_us(unsigned char tempo_final) {
  config_timer1(10);
  OCR1A = tempo_final * 2;         // Tempo final da contagem   *2
  while ((TIFR1 & (1 << 1)) == 0); // Contagens até o flag "zerar"
  TIFR1 = (1 << 1);                // limpa flag
}

void Timer1_ms(unsigned int tempo_ms) {
  config_timer1(1024);
  OCR1A = (unsigned short)(((unsigned long)(F_CPU / 1024) * tempo_ms) / 1000);
  while (!(TIFR1 & (1 << OCF1A))); // Enquanto o flag não for setado, espere.
  TIFR1 = (1 << OCF1A);            // limpa flag
}

void Timer1(unsigned char tempo_final) {
  config_timer1(13);
  OCR1A = tempo_final * 15625;     // Tempo final da contagem *15625
  while ((TIFR1 & (1 << 1)) == 0); // Contagens até o flag "zerar"
  TIFR1 = (1 << 1);                // limpa flag
}
