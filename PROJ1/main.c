#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/io.h>
#include <string.h>

#include "display.h"
#include "operacao.h"
#include "teclado.h"
#include "timer0_1.h"
#include "timer2.h"

// Variáveis globais
enum estado { desligado = 0, bloqueado = 1, operacao = 2 };
enum estado estadoAtual = desligado; // estado inicial desligado
char teclaPressionada;
char senha_digitada[5] = {'F', 'F', 'F', 'F', '\0'}; // O vetor já é instanciado para evitar lixo de memória
char senha_op1[5] = "3258";                          // Senha do operador 1
char senha_op2[5] = "8741";                          // Senha do operador 2
char operador_atual = 0; // Variável para armazenar qual operador está logado (0 - nenhum, 1 - op1, 2 - op2)

void desligado_loop() {
  static float inicio = 0;
  if (teclaPressionada == '#') {
    if (inicio == 0) {
      // Começa a contar o tempo no primeiro instante da pressão
      inicio = get_elapsed_time_ms();
    }

    // Verifica há quanto tempo está pressionada
    float tempoPressionado = get_elapsed_time_ms() - inicio;
    if (tempoPressionado >= 3000) {
      estadoAtual = bloqueado;
      inicio = 0; // Reseta o contador
    }

  } else {
    // Soltou a tecla: reseta o contador
    inicio = 0;
  }
}

void bloqueado_loop() {
  // Senha operador1: “3258” pode mudar configurações do sistema
  // Senha operador2: “8741”
  static int digitos = 0;

  LCD_Clear();
  LCD_String("Digite a senha:");
  for (int i = 0; i < digitos; i++) {
    LCD_String_xy(1, i, "*"); // Para cada dígito, mascara com * na tela
  }

  // Laço for para inserir a senha; a senha digitada é armazenada até terminar os 4 dígitos
  if (teclaPressionada != 'f' && teclaPressionada != '#' && teclaPressionada != '*' && digitos < 4) {
    senha_digitada[digitos] = teclaPressionada;
    digitos++;
    Timer1_ms(100);
  }

  // Faz a verificação dos digitos, retorna se ainda não foram digitados os 4 dígitos
  if (digitos < 4) {
    return;
  }

  LCD_Clear();
  if (strcmp(senha_digitada, senha_op1) == 0) // Verifica se a senha digitada é a senha do operador 1
  {
    operador_atual = 1; // Atualiza para operador 1
    LCD_String("Operador 1");
    Timer1_ms(3000);
  } else if (strcmp(senha_digitada, senha_op2) == 0) // Verifica se a senha digitada é a senha do operador 2
  {
    operador_atual = 2; // Atualiza para operador 2
    LCD_String("Operador 2");
    Timer1_ms(3000);
  } else {
    LCD_Clear();
    LCD_String("Acesso invalido");
    Timer1_ms(2000);
    LCD_Clear();
    LCD_String("Tente novamente");
    Timer1_ms(2000);
    LCD_Clear();

    digitos = 0; // Reseta o contador de dígitos para nova tentativa
    // Limpa o vetor de senha digitada para uma que possa ser inserida uma senha válida
    senha_digitada[0] = 'F';
    senha_digitada[1] = 'F';
    senha_digitada[2] = 'F';
    senha_digitada[3] = 'F';
    senha_digitada[4] = '\0';
    return;
  }

  // Se a senha estiver correta, desbloqueia o sistema
  LCD_Clear();
  LCD_String("Senha correta");
  Timer1_ms(1500);
  LCD_Clear();
  LCD_String("Desbloqueado");
  Timer1_ms(1500);
  LCD_Clear();
  LCD_String("Uber DeLEats");
  Timer1_ms(1500);
  LCD_Clear();

  if (operador_atual == 1) // Se for a senha do operador 1
  {
    LCD_String("Operador 1");
    Timer1_ms(1500);
    // Transm_estado_veiculo('1'); // Transmite mensagem serial de estado do carro como disponivel (1)
  } else if (operador_atual == 2) // Se for a senha do operador 2
  {
    LCD_String("Operador 2");
    Timer1_ms(1500);
    // Transm_estado_veiculo('1'); // Transmite mensagem serial de estado do carro como disponivel (1)
  }

  estadoAtual = operacao; // Atualiza para sistema desbloqueado
}

void desligar_sistema() {
  static float inicio = 0;
  if (teclaPressionada == '*') {
    if (inicio == 0) {
      // Começa a contar o tempo no primeiro instante da pressão
      inicio = get_elapsed_time_ms();
    }

    // Verifica há quanto tempo está pressionada
    float tempoPressionado = get_elapsed_time_ms() - inicio;
    if (tempoPressionado >= 4000) {
      LCD_String_xy(1, 0, "Desligando...");
      Timer1_ms(2000);
      LCD_Clear();
      estadoAtual = desligado;
      // desligar o DISPLAY
      inicio = 0; // Reseta o contador
    }

  } else {
    // Soltou a tecla: reseta o contador
    inicio = 0;
  }
}

int main() {
  teclado_init();
  timer2_init();
  LCD_Init();

  int fora_de_operacao = 1;
  while (fora_de_operacao) {
    teclaPressionada = tecla();
    desligar_sistema();

    switch (estadoAtual) {
    case desligado:
      desligado_loop();
      break;
    case bloqueado:
      bloqueado_loop();
      break;
    case operacao:
      fora_de_operacao = 0;
      break;
    }
  }

  return operacao_loop();
}