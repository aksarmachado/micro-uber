/*
 * timer.h
 *
 * Created: 18/10/2025 19:03:01
 *  Author: Aksa
 */

#ifndef TIMER_H_
#define TIMER_H_

void timer1_init();
// Função para obter o tempo decorrido em milissegundos desde a inicialização do timer
// Ex: chama uma vez para obter um inicio de tempo e depois chama novamente para obter o tempo final
// A diferença entre os dois valores retornados será o tempo decorrido em ms
uint32_t get_elapsed_time_ms();

#endif /* TIMER_H_ */