
#include "desligar.h"
#include "display.h"
#include "estados.h"
#include "timer0_1.h"
#include "timer2.h"

enum estado desligar_sistema(enum estado estadoAtual, char teclaPressionada) {
  static float inicio = 0;
  if (teclaPressionada == '*') {
    if (inicio == 0) {
      // Começa a contar o tempo no primeiro instante da pressão
      inicio = get_elapsed_time_ms();
    }

    // Verifica há quanto tempo está pressionada
    float tempoPressionado = get_elapsed_time_ms() - inicio;
    if (tempoPressionado >= 4000) {
      LCD_Clear();
      LCD_String("Desligando...");
      Timer1_ms(2000);
      LCD_Clear();
      LCD_Off();
      inicio = 0; // Reseta o contador
      return desligado;
    }

  } else {
    // Soltou a tecla: reseta o contador
    inicio = 0;
  }

  return estadoAtual;
}