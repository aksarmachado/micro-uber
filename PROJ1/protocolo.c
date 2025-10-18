/*
 * Protocolo.c
 *
 * Created: 11/10/2025 19:22:17
 *  Author: Aksa
 */

#include "serial.h"

// Fun��o para transmitir a mensagem de qual o movimento do veiculo por
// comunica��o serial para o servidor externo
void transmite_mov_veiculo(char mov) {
  char envia[4] = {0}; // Vetor que receber� a mensagem de qual movimento o carro ter�

  envia[0] = 'U'; // Mensagem para ser transmitida indicando movimenta��o
  envia[1] = 'M';
  envia[2] = mov; // Indica qual o movimento do carro, sendo 0-parado, 1-move
                  // p/ frente, 2-vira p/ direita, 3-vira p/ esquerda
  envia[3] = '\0';

  transmite_string_serial(envia,
                          3); // transmite mensagem de qual o movimento o carro ter� pela serial
}

// Fun��o para transmitir a mensagem de aceite do cliente por comunica��o serial
// para o servidor externo(recebe o byte contendo o codigo do cliente)
void transmite_aceita_cliente(char cliente) {
  char envia[4] = {0}; // Vetor que receber� a mensagem de qual movimento o carro ter�

  envia[0] = 'U'; // Mensagem para ser transmitida indicando aceite de cliente
  envia[1] = 'A';
  envia[2] = cliente; // Aloca  conteudo do byte do cliente no terceiro byte da
                      // mensagem a ser transmitida
  envia[3] = '\0';

  transmite_string_serial(envia,
                          3); // transmite mensagem de aceite do cliente para o servidor externo
  // escreve_string_lcd("SA");			// mensagem para sinalizar o
  // cliente aceito no LCD _delay_ms(200); limpa_display();
}

void transmite_estado_veiculo(char status_veiculo) {
  char envia[4] = {0}; // vetor em que é alocado a mensagm de estado do carro

  envia[0] = 'U';
  envia[1] = 'E';            // mensagem de estado do veículo
  envia[2] = status_veiculo; // 0 - Indisponível, 1 - Disponível, 2 - Ocupado
  envia[3] = '\0';

  transmite_string_serial(envia,
                          3); // transmite mensagem de qual o estado do carro
}

// Função para transmitir a mensagem de corrida iniciada por comunicação serial
// para o servidor externo(recebe o byte contendo o codigo do cliente)
void transmite_inicio_corrida(char cliente) {
  char envia[4] = {0}; // Vetor que receberá a mensagem de qual movimento o carro terá

  envia[0] = 'U'; // Mensagem para ser transmitida indicando inicio de corrida
  envia[1] = 'I';
  envia[2] = cliente; // Aloca  conteudo do byte do cliente no terceiro byte da
                      // mensagem a ser transmitida
  envia[3] = '\0';

  transmite_string_serial(envia, 3); // transmite mensagem de inicio de corrida
  // escreve_string_lcd("SI");			// mensagem para sinalizar
  // inicio de corrida no LCD _delay_ms(200); limpa_display();
}

// Função para transmitir a mensagem de fim de corrida por comunicação serial
// para o servidor externo(recebe o byte contendo o codigo do cliente)
void transmite_final_corrida(char cliente) {
  char envia[4] = {0}; // Vetor que receberá a mensagem de qual movimento o carro terá

  envia[0] = 'U'; // Mensagem para ser transmitida indicando fim de corrida
  envia[1] = 'F';
  envia[2] = cliente; // Aloca  conteudo do byte do cliente no terceiro byte da
                      // mensagem a ser transmitida
  envia[3] = '\0';

  transmite_string_serial(envia, 3); // transmite mensagem de fim de corrida
  // escreve_string_lcd("SF");				// mensagem para
  // sinalizar fim de corrida no LCD _delay_ms(200); limpa_display();
}
