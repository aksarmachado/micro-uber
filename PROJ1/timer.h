/*
 * timer.h
 *
 * Created: 18/10/2025 19:03:01
 *  Author: Aksa
 */

#ifndef TIMER_H_
#define TIMER_H_

void timer2_init();
// Função para obter o tempo decorrido em milissegundos desde a inicialização do timer
// Ex: chama uma vez para obter um inicio de tempo e depois chama novamente para obter o tempo final
// A diferença entre os dois valores retornados será o tempo decorrido em ms
float get_elapsed_time_ms();

void config_timer0(unsigned char p);
void Timer0_ms(unsigned char tempo_final);
void Timer1_us(unsigned char tempo_final);
void Timer1_ms(unsigned char tempo_final);
void Timer1(unsigned char tempo_final);
void config_timer1(unsigned char p);

#endif /* TIMER_H_ */