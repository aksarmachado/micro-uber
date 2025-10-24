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

#include "display.h"
#include "protocolo.h"
#include "serial.h"
#include "teclado.h"
#include "timer.h"
#include <string.h>
#include <util/delay.h>

// Variáveis globais
enum estado { desligado = 0, bloqueado = 1, disponivel = 2, operacao = 3, movimento = 4 };
enum estado estadoAtual = desligado; // estado inicial desligado
char teclaPressionada;
char senha_digitada[5] = {'F', 'F', 'F', 'F',
                          '\0'}; // O vetor que receberá a senha digitada já é instanciado para evitar lixo de memória
char senha_op1[5] = "3258";      // Senha do operador 1
char senha_op2[5] = "8741";      // Senha do operador 2
char operador_atual = 0;         // Variável para armazenar qual operador está logado (0 - nenhum, 1 - op1, 2 - op2)

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
      disponivel_loop();
      break;
    case operacao:
      // operacao_loop();
      break;
    case movimento:
      // movimento_loop();
      break;
    default:
      // desligado_loop();
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

  if (teclaPressionada == '#') {
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

void bloqueado_loop() {

  // esperar senha  1 ou 2
  // Senha operador1: “3258” pode mudar configurações do sistema
  //  Senha operador2: “8741”

  char tecla = 0;
  int digitos = 0, i = 0;

  // Permanece em um loop até que algum usuário seja acessado, alterando o valor de status[0] para um valor diferente de
  // 0, indicando se o status[0] está bloqueado (0) ou desbloqueado (1).
  while (estadoAtual == 0) {
    limpa_display();
    escreve_string_lcd("Digite a senha:");
    escreve_comando_lcd(0xC0); // Segunda linha do display

    // Laço for para inserir a senha; a senha digitada é armazenada na string senha_digitada.
    for (digitos = 0; digitos < 4; digitos++) {
      // Enquanto nenhuma tecla for pressionada fica em loop, se nenhuma tecla foi pressionada senha esta com seu valor
      // original ou com zero
      while (senha_digitada[digitos] == 0 || senha_digitada[digitos] == 'F') {
        // Prende nesse laco enquanto não for´pressionada nenhuma tecla
        while (!tecla) {
          tecla = teclaPressionada;
        }
        senha_digitada[digitos] = tecla; // Até terminar os 4 dígitos vai armazenando os números das teclas pressionadas
                                         // na string senha_digitada, dígito a dígito
        escreve_dado_char_lcd('*');      // Para cada dígito, mascara com * na tela
        _delay_ms(200);
        tecla = 0;
      }
    }

    // Faz a verificação da senha

    if (strcmp(senha_digitada, senha_op1) == 0) // Verifica se a senha digitada é a senha do operador 1
    {
      estadoAtual = 2;                                 // Atualiza para sistema desbloqueado
      operador_atual = 1;                              // Atualiza para operador 1
    } else if (strcmp(senha_digitada, senha_op2) == 0) // Verifica se a senha digitada é a senha do operador 2
    {
      estadoAtual = 2;    // Atualiza para sistema desbloqueado
      operador_atual = 2; // Atualiza para operador 2
    }

    limpa_display();

    // Se o estado_sistema[0] for 1, testa qual o usuário e imprime na tela operador correspondente
    if (estadoAtual == 2) {
      if (operador_atual == 1) // Se for a senha do operador 1
      {
        limpa_display();
        escreve_string_lcd("Senha correta");
        escreve_comando_lcd(0xC0); // Pula uma linha no display
        escreve_string_lcd("Desbloqueado");

        _delay_ms(1000);
        limpa_display();
        escreve_string_lcd("     Uber DeLEats     ");
        escreve_comando_lcd(0xC0); // Pula uma linha no display
        escreve_string_lcd("   Bem Vindo!   ");

        _delay_ms(500);
        limpa_display();
        escreve_string_lcd("   Operador 1");
        _delay_ms(250);
        Transm_estado_veiculo('1'); // Transmite mensagem serial de estado do carro como disponivel (1)

      } else if (operador_atual == 2) // Se for a senha do operador 2
      {
        limpa_display();
        escreve_string_lcd("Senha correta");
        escreve_comando_lcd(0xC0); // Pula uma linha no display
        escreve_string_lcd("Desbloqueado");

        _delay_ms(1000);
        limpa_display();
        escreve_string_lcd("     Uber DeLEats     ");
        escreve_comando_lcd(0xC0);
        escreve_string_lcd("   Bem Vindo!   ");

        _delay_ms(500);
        limpa_display();
        escreve_string_lcd("   Operador 2");
        _delay_ms(250);
        Transm_estado_veiculo('1'); // Transmite mensagem serial de estado do carro como disponivel (1)
      }

    }
    // Caso contrario, nenhuma senha de uso foi digitada, e o loop volta a pedir para alguma senha ser digitada
    else {
      limpa_display();
      escreve_string_lcd("Acesso invalido");
      escreve_comando_lcd(0xC0); // Pula uma linha no display
      escreve_string_lcd("Tente novamente");
      digitos = 0;
      // Limpa o vetor de senha digitada para uma que possa ser inserida uma senha válida
      senha_digitada[0] = 'F';
      senha_digitada[1] = 'F';
      senha_digitada[2] = 'F';
      senha_digitada[3] = 'F';
      senha_digitada[4] = '\0';
    }

    _delay_ms(3000);
    limpa_display();
  }
}
void disponivel_loop() {
  // código para o estado disponível
  // espero receber chamdada por serial
  // aceito ou não as chamadas
}
