/*
 * GccApplication2.c
 *
 * Created: 19/09/2025 14:37:34
 * Author : Aksa e Gabriel 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int BOUNCE = 8, Flag = 0;

void atraso_debounce(){
	while((TIFR0 & (1 << 0)) == 0); //enquanto a flag de interrupção do timer 0 não for 1
	TCNT0 = 31;  
	TIFR0 |= (1 << 0);  // zera a flag 
}

void atraso_TIM1_1100Hz(){
	while((TIFR1 & (1 << 0)) == 0);
	TCNT1 = 58263;
	TIFR1 |= (1 << 0);
}

void atraso_TIM1_2200Hz(){
	while((TIFR1 & (1 << 0)) == 0);
	TCNT1 = 61899;
	TIFR1 |= (1 << 0);
}

//Função de debounce para leitura do teclado, a função recebe como argumento a coluna da matriz de contato
char debounce (char COLUNA){
	
	char count = 0, key_last = 0, key_now = 0;
	
	while(1){
		atraso_debounce();

		if(COLUNA == 0) key_now = (PINH & (1 << PH3));
		if(COLUNA == 1) key_now = (PINH & (1 << PH4));
		if(COLUNA == 2) key_now = (PINH & (1 << PH5));

		if(key_now == key_last){count = count + 1;}
		else{count = 0;}

		if(count == BOUNCE){
			if (key_now) return 1;
			else return 0;
			}
		else{key_last = key_now;}
	}
}

ISR(TIMER1_OVF_vect){
	TIFR1 = (1 << 0);
	
	if (Flag == 1) {TCNT1 = 58263;}
	else if (Flag == 2) {TCNT1 = 61899;}
	else {TCNT1 = 0;
		  PORTB &=~ 1 << 7;
		  return;}
	
	PORTB ^= 1 << 7;
	}

int main(void)
{
	PINH = 255;  // define a porta H com pull down
	
	DDRB |= (1<<7);  //define PB7 como saída
	
    DDRH &=~ ((1 << PH3) | (1 << PH4) | (1 << PH5));  //define PH3, PH4 e PH5 como entradas
	
	TCCR0A = 0;
	TCCR0B = 4;
	TCNT0 = 31;
	
 	TCCR1A = 0;
 	TCCR1B = 1;
 	TCNT1 = 58263;
	 
	TIMSK1 = 1 << 0;
	 
	sei();
	
    while (1) {
		if (!debounce(0)) {
			Flag = 1;
	    }

	    if (!debounce(1)) {
			Flag = 2;
	    }

	    if (!debounce(2)) {
			Flag = 3;
	    }

    }
}

