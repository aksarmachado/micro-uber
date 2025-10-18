/*
 * Serial.h
 *
 * Created: 11/10/2025 16:38:52
 *  Author: Aksa
 */

#ifndef SERIAL_H_
#define SERIAL_H_

void limpa_serial();
void serial_init();
char recepcao_serial_dado_char();
char recebe_string_serial(char *dado_recebido, int tamanho);
void transmite_string_serial(char *dado_transmitir, int tamanho);

#endif /* SERIAL_H_ */