#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>
#include <stdio.h> // importante para sprintf
#include <stdlib.h>

#include "desligar.h"
#include "display.h"
#include "estados.h"
#include "teclado.h"
#include "timer0_1.h"

unsigned char id_cliente = 0, novo_cliente = 0;

unsigned char posx_entregador[2], posy_entregador[2];
unsigned char posx_coleta[2], posy_coleta[2];
unsigned char posx_entrega[2], posy_entrega[2];

unsigned int x = 0, y = 0;
unsigned int x_coleta = 0, y_coleta = 0;
unsigned int x_entrega = 0, y_entrega = 0;

unsigned int indice = 0, dif_x_entregador = 0, dif_y_entregador = 0, dif_x_coleta = 0, dif_y_coleta = 0,
             dif_x_entrega = 0, dif_y_entrega = 0;
unsigned int esq_x_entregador = 0, esq_y_entregador = 0, esq_x_coleta = 0, esq_y_coleta = 0, esq_x_entrega = 0,
             esq_y_entrega = 0;

unsigned char data[2];
unsigned char hora[2];

char texto_3[5];

unsigned char estado = 0, estados_1 = 1;
unsigned char contador = 0;
unsigned char contador_d = 0, contador_c = 0, contador_e = 0;
int pedido_coletado = 0, pedido_coletado_1 = 0, pedido_entregue = 0, pedido_finalizado = 0;

int entra_uma_vez_1 = 1, entra_uma_vez_2 = 1, entra_uma_vez_3 = 1;

int direcao_motoqueiro = 1; //{1, 2, 3, 4}; // 1 - Frente, 2 - Tr�s, 3 - Direita e 4 - Esquerda

int esquinas[9][2] = {{620, 680},   {1220, 680}, {1820, 680},  {620, 1360}, {1220, 1360},
                      {1820, 1360}, {620, 2040}, {1220, 2040}, {1820, 2040}};

int indiceMaisProximo(int x, int y) {
  int indice = 0;
  double menor = sqrt(pow(x - esquinas[0][0], 2) + pow(y - esquinas[0][1], 2));
  for (int i = 1; i < 9; i++) {
    double d = sqrt(pow(x - esquinas[i][0], 2) + pow(y - esquinas[i][1], 2));
    if (d < menor) {
      menor = d;
      indice = i;
    }
  }
  return indice;
}

// int siga_em_frente = 0, virar_esquerda = 0, virar_direita = 0, meia_volta = 0;

int decidirMovimento(int x_moto, int y_moto, int x_col, int y_col) {
  // int siga_em_frente = 0, virar_esquerda = 0, virar_direita = 0, meia_volta = 0;

  if (y_moto > y_col)
    // siga_em_frente = 1;
    direcao_motoqueiro = 1;
  else
    direcao_motoqueiro = 2;
  // meia_volta = 1;

  if (x_col < x_moto)
    // virar_esquerda = 1;
    direcao_motoqueiro = 4;
  else if (x_col > x_moto)
    // virar_direita = 1;
    direcao_motoqueiro = 3;

  // Exemplo de uso
  // if (siga_em_frente) printf("Siga em frente\n");
  // if (meia_volta)     printf("D� meia volta\n");
  // if (virar_esquerda) printf("Vire � esquerda\n");
  // if (virar_direita)  printf("Vire � direita\n");

  return direcao_motoqueiro;
}

char tecla_pressionada;

int msg_pronta = 0, msg_data_hora = 0, disponivel_coleta = 0, aceita_pedido = 0, indisponivel = 0, valida_posicao = 0,
    habilita_dir_teclado;

int difx = 0, dify = 0;

void envia2bytes(char a, char b) {
  // Envia o primeiro byte
  UDR0 = a;
  while ((UCSR0A & (1 << TXC0)) == 0); // Espera a transmiss�o terminar
  UCSR0A |= (1 << TXC0);               // Limpa o flag de transmiss�o

  // Envia o segundo byte
  UDR0 = b;
  while ((UCSR0A & (1 << TXC0)) == 0); // Espera a transmiss�o terminar
  UCSR0A |= (1 << TXC0);               // Limpa o flag novamente
}

void envia3bytes(char a, char b, char c) {
  // Envia o primeiro byte
  UDR0 = a;
  while ((UCSR0A & (1 << TXC0)) == 0); // Espera a transmiss�o terminar
  UCSR0A |= (1 << TXC0);               // Limpa o flag de transmiss�o

  // Envia o segundo byte
  UDR0 = b;
  while ((UCSR0A & (1 << TXC0)) == 0); // Espera a transmiss�o terminar
  UCSR0A |= (1 << TXC0);               // Limpa o flag novamente

  // Envia o segundo byte
  UDR0 = c;
  while ((UCSR0A & (1 << TXC0)) == 0); // Espera a transmiss�o terminar
  UCSR0A |= (1 << TXC0);               // Limpa o flag novamente
}

void enviamov(int a) {
  // Envia o primeiro byte
  UDR0 = 'U';
  while ((UCSR0A & (1 << TXC0)) == 0); // Espera a transmiss�o terminar
  UCSR0A |= (1 << TXC0);               // Limpa o flag de transmiss�o

  // Envia o segundo byte
  UDR0 = 'M';
  while ((UCSR0A & (1 << TXC0)) == 0); // Espera a transmiss�o terminar
  UCSR0A |= (1 << TXC0);               // Limpa o flag novamente

  UDR0 = a;
  while ((UCSR0A & (1 << TXC0)) == 0); // Espera a transmiss�o terminar
  UCSR0A |= (1 << TXC0);               // Limpa o flag novamente
}

void uart_init() {
  // Configura UART para 19200 bps, 8N1 @ 16 MHz
  UBRR0H = 0;
  UBRR0L = 51;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // RX, TX e interrup��o RX habilitados
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void pos_x_y() {
  x = ((unsigned int)posx_entregador[0] << 8) | posx_entregador[1];
  y = ((unsigned int)posy_entregador[0] << 8) | posy_entregador[1];

  x_coleta = ((unsigned int)posx_coleta[0] << 8) | posx_coleta[1];
  y_coleta = ((unsigned int)posy_coleta[0] << 8) | posy_coleta[1];

  x_entrega = ((unsigned int)posx_entrega[0] << 8) | posx_entrega[1];
  y_entrega = ((unsigned int)posy_entrega[0] << 8) | posy_entrega[1];

  if (((x - x_coleta) <= 80) && ((y - y_coleta) <= 80) && entra_uma_vez_1) { // entra_uma_vez regula a entrada no offset
    envia3bytes('U', 'I', id_cliente);
    pedido_coletado = 1;
    entra_uma_vez_1 = 0;
    enviamov(1);
  }

  if (((x - x_entrega) <= 100) && ((y - y_entrega) <= 100) &&
      pedido_coletado_1) { // colocar flag de finaliza��o para evitar loops
    envia3bytes('U', 'F', id_cliente);
    pedido_coletado_1 = 0;
    pedido_entregue = 1;
    enviamov(1);
  }
}

void esq_proxima() {
  pos_x_y();

  indice = indiceMaisProximo(x, y);
  dif_x_entregador = abs(esquinas[indice][0] - x);
  // dif_y_entregador = abs (esquinas[indice][1] - y);
  esq_x_entregador = esquinas[indice][0];
  esq_y_entregador = esquinas[indice][1];

  indice = indiceMaisProximo(x_coleta, y_coleta);
  dif_x_coleta = abs(esquinas[indice][0] - x_coleta);
  // dif_y_coleta = abs (esquinas[indice][1] - y_coleta);
  esq_x_coleta = esquinas[indice][0];
  esq_y_coleta = esquinas[indice][1];

  indice = indiceMaisProximo(x_entrega, y_entrega);
  dif_x_entrega = abs(esquinas[indice][0] - x_entrega);
  // dif_y_entrega = abs (esquinas[indice][1] - y_entrega);
  esq_x_entrega = esquinas[indice][0];
  esq_y_entrega = esquinas[indice][1];
}

void direcoes(int a, int b) { // Calculo das distancias envolvidas e dire��o do motoqueiro
  habilita_dir_teclado = 1;
  char disty[5];

  pos_x_y();
  esq_proxima();

  if (b) {
    switch (estados_1) {
    case 1:
      LCD_Clear();
      LCD_String("Siga reto por:");
      LCD_Command(0xC0);
      sprintf(disty, "%d", abs(esq_y_coleta - y));
      LCD_String(disty);
      LCD_String("m");
      break;

    case 2:
      LCD_Clear();
      LCD_String("Siga reto por:");
      LCD_Command(0xC0);
      sprintf(disty, "%d", (abs(esq_x_coleta - x)));
      LCD_String(disty);
      LCD_String("m");
      break;

    case 3:
      LCD_Clear();
      LCD_String("Siga reto por:");
      LCD_Command(0xC0);
      sprintf(disty, "%d", abs(esq_y_entrega - y));
      LCD_String(disty);
      LCD_String("m");
      break;

    case 4:
      LCD_Clear();
      LCD_String("Siga reto por:");
      LCD_Command(0xC0);
      sprintf(disty, "%d", (abs(esq_x_entrega - x)));
      LCD_String(disty);
      LCD_String("m");
      break;

    case 5:
      LCD_Clear();
      LCD_String("Siga reto por:");
      LCD_Command(0xC0);
      sprintf(disty, "%d", abs(x_entrega - x));
      LCD_String(disty);
      LCD_String("m");
      break;

    case 6:
      LCD_Clear();
      LCD_String("Siga reto por:");
      LCD_Command(0xC0);
      sprintf(disty, "%d", (abs(y_entrega - y)));
      LCD_String(disty);
      LCD_String("m");
      break;

    case 7:
      LCD_Clear();
      LCD_String("Siga reto por:");
      LCD_Command(0xC0);
      sprintf(disty, "%d", abs(x_coleta - x));
      LCD_String(disty);
      LCD_String("m");
      break;

    case 8:
      LCD_Clear();
      LCD_String("Siga reto por:");
      LCD_Command(0xC0);
      sprintf(disty, "%d", (abs(y_coleta - y)));
      LCD_String(disty);
      LCD_String("m");
      break;
    }
  }

  if ((abs(esq_y_coleta - y) <= 30) && !pedido_coletado && (abs(x - x_coleta) >= 50)) {
    estados_1 = 7;

    LCD_Clear();
    if (x_coleta < x) {
      LCD_String("Vire a esquerda");
    } else {
      LCD_String("Vire a direita");
    }

    Timer1(2);
  }

  // else if ((abs(esq_x_coleta - x) <= 30) && !pedido_coletado && (abs(y - y_coleta) >= 50)) {
  // estados_1 = 8;
  //}
  //
  // else if ((abs(esq_x_coleta - x) >= 50) && !pedido_coletado && (abs(y - y_coleta) >= 50)) {
  // estados_1 = 1; //caso esteja perdido, vai ate a esquina mais proxima, e ai cai num caso conhecido
  //}

  else if ((abs(esq_y_entrega - y) <= 30) && pedido_coletado_1 && (abs(x - x_entrega) >= 50)) {
    estados_1 = 5;

    LCD_Clear();
    if (x_entrega < x) {
      LCD_String("Vire a esquerda");
    } else {
      LCD_String("Vire a direita");
    }
    Timer1(2);
  } else if ((abs(esq_x_entrega - x) <= 30) && pedido_coletado_1 && (abs(y - y_entrega) >= 50)) {
    estados_1 = 6;

    LCD_Clear();
    if (y_entrega > y) {
      LCD_String("Vire a esquerda");
    } else {
      LCD_String("Vire a direita");
    }
    Timer1(2);
  }

  else if ((abs(esq_x_entrega - x) >= 50) && pedido_coletado_1 && (abs(y - y_entrega) >= 50)) {
    estados_1 = 3;

    if (entra_uma_vez_2) {
      entra_uma_vez_2 = 0;
      LCD_Clear();
      if (y_entrega > y) {
        LCD_String("Vire a esquerda");
      } else {
        LCD_String("Vire a direita");
      }
      Timer1(2);
    }
  }

  if (pedido_coletado) {
    pedido_coletado = 0;

    LCD_Clear();
    LCD_String("Pedido coletado!");
    enviamov(1);
    Timer1(1);
  } else if (pedido_entregue) {
    pedido_entregue = 0;

    LCD_Clear();
    LCD_String("Pedido entregue!");
    // while(pedido_finalizado); //Retirar ou nao, eis a questao //retirado, respondida a questao
  }
}

void valida_coleta() {
  if (aceita_pedido) {
    aceita_pedido = 0;

    LCD_Clear();
    LCD_String("Realize a coleta");

    pos_x_y();

    Timer1(1);
    direcoes(y, y_coleta);
  }
}

void negar_coleta() {
  if (aceita_pedido) {
    LCD_Clear();
    LCD_String("Sem sucesso");
  }
}

void cliente_novo() {
  envia2bytes('U', 'E');

  LCD_Clear();

  LCD_String("Novo pedido: ");
  LCD_Command(0xC0);
  LCD_String("Aceitar 1-S  2-N");

  while ((tecla_pressionada != '1') && (tecla_pressionada != '2')) {
    tecla_pressionada = tecla();

    if (tecla_pressionada == '1') {
      envia3bytes('U', 'A', id_cliente);
      aceita_pedido = 1;
    } else if (tecla_pressionada == '2') {
      LCD_Clear();
      LCD_String("Pedido rejeitado");
    }
  }
}

// ISR de recep��o serial
ISR(USART0_RX_vect) {
  unsigned char dado = UDR0;

  switch (estado) {
  case 0: // Espera 'S'
    if (dado == 'S') {
      estado = 1;
    }
    break;

  case 1: // Espera 'P'
    if (dado == 'P') {
      pedido_finalizado = 0;
      estado = 2;
      contador = 0;
    } else if (dado == 'H') {
      estado = 3;
      contador_d = 0;
    } else if (dado == 'E') {
      estado = 4;
      contador_c = 0;
      contador_e = 0;
    } else if (dado == 'A') {
      estado = 0;
      valida_coleta();
    } else if (dado == 'N') {
      estado = 0;
      negar_coleta();
    } else if (dado == 'M') {
      estado = 0;
      valida_posicao = 1;
    } else if (dado == 'I') {
      estado = 0;
      pedido_coletado_1 = 1;
    } else if (dado == 'F') {
      pedido_finalizado = 1;
      entra_uma_vez_1 = 1;
      entra_uma_vez_2 = 1;
    } else {
      estado = 0;
    }
    break;

  case 2: // Lendo 4 bytes de dados
    if (contador < 2) {
      posx_entregador[contador] = dado;
    } else {
      posy_entregador[contador - 2] = dado;
    }
    contador++;

    if (contador >= 4) {
      msg_pronta = 1;
      estado = 0; // volta a procurar nova mensagem
    }
    break;

  case 3: // Lendo data e hora
    if (contador_d < 2) {
      data[contador_d] = dado;
    } else {
      hora[contador_d - 2] = dado;
    }
    contador_d++;

    if (contador_d >= 4) {
      msg_data_hora = 1;
      estado = 0; // volta a procurar nova mensagem
    }
    break;

  case 4: // Lendo id
    novo_cliente = 1;
    id_cliente = dado;
    estado = 5;
    break;

  case 5: // Lendo posx, posy entrega
    if (contador_e < 2) {
      posx_entrega[contador_e] = dado;
    } else {
      posy_entrega[contador_e - 2] = dado;
    }
    contador_e++;

    if (contador_e >= 4) {
      estado = 6; // volta a procurar nova mensagem
    }
    break;

  case 6: // Lendo posx, posy coleta
    if (contador_c < 2) {
      posx_coleta[contador_c] = dado;
    } else {
      posy_coleta[contador_c - 2] = dado;
    }
    contador_c++;

    if (contador_c >= 4) {
      estado = 0;
    }
    break;

  default:
    estado = 0;
  }
}

enum estado operacao_loop() {
  char texto_1[10]; // buffer para armazenar o n�mero convertido
  enum estado estadoAtual = operacao;

  uart_init();

  while (1) {

    if (novo_cliente) {
      novo_cliente = 0;
      cliente_novo();
    }

    if (msg_pronta) {
      msg_pronta = 0;

      envia2bytes('U', 'P');

      pos_x_y();

      if (!pedido_coletado && !pedido_finalizado)
        direcoes(y, y_coleta);
      if (!entra_uma_vez_1 && !pedido_entregue && !pedido_finalizado)
        direcoes(y, y_entrega);
    }

    if (msg_data_hora) {
      msg_data_hora = 0;

      envia2bytes('U', 'H');

      LCD_Command(0xC0);
      sprintf(texto_1, "%d", data[0]);
      LCD_String(texto_1);
      LCD_String("/");
      sprintf(texto_1, "%d", data[1]);
      LCD_String(texto_1);
      LCD_String("   ");
      sprintf(texto_1, "%d", hora[0]);
      LCD_String(texto_1);
      LCD_String(":");
      sprintf(texto_1, "%d", hora[1]);
      LCD_String(texto_1);
    }

    tecla_pressionada = tecla();
    estadoAtual = desligar_sistema(estadoAtual, tecla_pressionada);
    if (estadoAtual != operacao) {
      return estadoAtual;
    }

    if (habilita_dir_teclado) {
      tecla_pressionada = tecla();

      switch (tecla_pressionada) {
      case '2':
        enviamov(2);
        direcao_motoqueiro = 1;
        break;
      case '5':
        enviamov(1);
        break;
      case '6':
        enviamov(3);
        direcao_motoqueiro = 3;
        break;
      case '4':
        enviamov(4);
        direcao_motoqueiro = 4;
        break;
      }
    }
  }
}
