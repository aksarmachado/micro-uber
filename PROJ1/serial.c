/*
 * Serial.c
 *
 * Created: 11/10/2025 15:34:26
 *  Author: Aksa
 */
//---------------FUN��ES PARA A COMUNICA��O SERIAL---------------
#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

// limpa o buffer de recep��o serial
void limpa_serial() {
  char limpa_recepcao;

  while (UCSR0A & (1 << RXC0)) // limpa e descarta at� n�o ter nada na recep��o
  {
    limpa_recepcao = UDR0;
  }
}

void serial_init() {

  /* baud rate 4800 */
  UBRR0H = 0;
  UBRR0L = 51; // Para f_osc = 16MHz, 19200 bps

  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // pinos 4, 3 e 7 - Habilita recep��o, transmiss�o serial e
                                                        // interrup��o por recep��o serial da serial  0
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

  UCSR0C = (1 << 5) + (1 << 4) + 0x6; // formato: 8 + 1 stop bit  + paridade impar

  limpa_serial();
}
// Fun��o que recebe um byte por comunica��o serial
char recepcao_serial_dado_char() {
  while (!(UCSR0A & (1 << RXC0))); // Aguarda o bit RXC ficar em 1 indicando que um novo byte foi recebido
  return UDR0;
}

// Fun��o que recebe um conjunto de bytes da serial, entrada � o vetor serial a ser lido e o tamanho do vetor (numero de
// bytes)
void recebe_string_serial(char *dado_recebido, int tamanho) {
  int i;

  for (i = 0; i < tamanho; i++) {
    dado_recebido[i] = recepcao_serial_dado_char();
  }
  dado_recebido[tamanho] = '\0';
}

// Fun��o que transmite um byte por comunica��o serial
void transmissao_serial_dado_char(char dado_transmitido) {
  while (!(UCSR0A & (1 << UDRE0))); // Enquanto o buffer de transmiss�o de dados n�o estiver vazio UDR0 ainda n�o
                                    // escreve os dados para transmitir
  UDR0 = dado_transmitido;
}

// Fun��o que transmite um conjunto de bytes da serial, entrada � o vetor serial a ser lido e o tamanho do vetor (numero
// de bytes)
void transmite_string_serial(char *dado_transmitir,
                             int tamanho) // essa fun��o vai transmitir estado para display ou interface
{
  int i;

  for (i = 0; i < tamanho; i++) {
    transmissao_serial_dado_char(dado_transmitir[i]);
  }
}
