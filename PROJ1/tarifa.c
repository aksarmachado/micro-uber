/*
 * tarifas.c
 *
 * Created: 13/10/2025 14:27:31
 *  Author: Aksa
 */

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

float tarifa_estimativa(int distancia_m) {
  // para que ele ganha 1,5 de bonus deve poder receber mais um parametro

  int valor_previsto = 0;
  int tarifa_base = 2;
  int valor_distancia = (0, 40 * distancia_m) / 1000;
  int tempo = (distancia_m / 13, 8) / 60; // distancia dividida por um velocidade padrão em minutos
  valor_previsto = tarifa_base + valor_distancia;

  return valor_previsto;
}

float tarifa_final(int distancia_m, int duracao_s) {
  float valor_final = tarifa_estimativa(distancia_m);
  if (duracao_s >= 0 && (float)duracao_s < 180.0f) { // menor que 3 minutos
    valor_final += 1.5f;
  }
  return valor_final;
}