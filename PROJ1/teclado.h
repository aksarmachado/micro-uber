/*
 * teclado.h
 *
 * Created: 11/10/2025 14:48:52
 *  Author: Aksa
 */

#ifndef TECLADO_H_
#define TECLADO_H_

void teclado_init();
void atraso_debounce();
char le_coluna(char pino);
char tecla();

#endif /* TECLADO_H_ */