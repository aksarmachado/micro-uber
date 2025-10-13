
#ifndef SERIAL_H_
#define SERIAL_H_

extern char envia_conf_cliente[3] = {'U', 'E', '\0'}; // Vetor que terá a mensagem de confirmação de recebimento
                                                      // da posição do cliente e do destino

extern char recebe_pos_carro[7] = {0}; //{'S', P', 'xp_alto', 'xp_baixo', 'yp_alto', 'yp_baixo'}, Vetor que
                                       // receberá a mensagem de que posição o carro se encontra

extern char envia_conf_pos[3] = {'U', 'P', '\0'};  // Vetor que terá a mensagem de confirmação de recebimento
                                                   // da posição
extern char envia_data_hora[3] = {'U', 'H', '\0'}; // Vetor que terá a mensagem de confirmação da data e hora do pedido

void transmite_mov_veiculo(char mov);
void transmite_aceita_cliente(char cliente);
void transmite_estado_veiculo(char status_veiculo);
void transmite_inicio_corrida(char cliente);

#endif /* SERIAL_H_ */