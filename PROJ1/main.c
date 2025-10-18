/*
 * TECLADO_PROJ1.c
 *
 * Created: 10/10/2025 16:13:15
 * Author : Aksa
 */

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/io.h>

#include "protocolo.h"
#include "serial.h"
#include "teclado.h"
#include "timer.h"
// Variáveis globais
enum estado { desligado = 0, bloqueado = 1, disponivel = 2, operacao = 3, movimento = 4 };
enum estado estadoAtual = desligado; // estado inicial desligado
char teclaPressionada;

int main() {
  teclado_init();
  timer1_init();

  while (1) {
    teclaPressionada = tecla();

    switch (estadoAtual) {
    case desligado:
      desligado_loop();
      break;
    case bloqueado:
      bloqueado_loop();
      break;
    case disponivel:
      //   estadoAtual = disponivel_loop();
      break;
    case operacao:
      //   estadoAtual = operacao_loop();
      break;
    case movimento:
      //   estadoAtual = movimento_loop();
      break;
    default:
      desligado_loop();
    }
  }

  // REMOVER ABAIXO - APENAS PARA TESTE DO TECLADO
  char flag = 0;
  DDRB = (1 << 7);
  // teste teclado
  if (tecla() == '1') {
    if (flag == 0)
      PORTB ^= 1 << 7;
    flag = 1;
  } else {
    flag = 0;
  }
  // fim do teste teclado
}

void desligado_loop() {
  static uint32_t inicio = 0;

  if (teclaPressionada == '3') {
    if (inicio == 0) {
      // Começa a contar o tempo no primeiro instante da pressão
      inicio = get_elapsed_time_ms();
    }

    // Verifica há quanto tempo está pressionada
    if (get_elapsed_time_ms() - inicio >= 3000) {
      estadoAtual = bloqueado;
      inicio = 0; // Reseta o contador
    }

  } else {
    // Soltou a tecla: reseta o contador
    inicio = 0;
  }
}

void bloqueado_loop() {}