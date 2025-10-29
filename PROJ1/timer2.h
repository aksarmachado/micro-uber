#ifndef TIMER2_H_
#define TIMER2_H_

void timer2_init();
// Função para obter o tempo decorrido em milissegundos desde a inicialização do timer
// Ex: chama uma vez para obter um inicio de tempo e depois chama novamente para obter o tempo final
// A diferença entre os dois valores retornados será o tempo decorrido em ms
float get_elapsed_time_ms();

#endif /* TIMER_H_ */