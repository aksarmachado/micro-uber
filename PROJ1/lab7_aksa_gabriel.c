/*
* LAB7.c
*
* Created: 26/09/2025 13:52:01
* Author : Aksa Machado e Gabriel Santos
*/

#include <avr/io.h>
#include <avr/interrupt.h>


void atraso_TIM1_1000Hz(){
	OCR1A = 7999;
	while((TIFR1 & (1<<1)) == 0);
	TIFR1 = (1<<1);
}

void atraso_TIM1_2000Hz(){
	OCR1A = 3999;
	while((TIFR1 & (1<<1)) == 0);
	TIFR1 = (1<<1);
}

void atraso_TIM1_4000Hz(){
	OCR1A = 1999;
	while((TIFR1 & (1<<1)) == 0); 
	TIFR1 = (1<<1);
}


int main(void)
{
	
	char dado = 0;
	
    DDRB = (1<<7);  //define PB7 como saída
	
	TCCR1A = 0;  // modo CTC 
	TCCR1B = 1 + (1<<3) ;  //prescaler 1

	/* baud rate 4800 */
	UBRR0H = 0;
	UBRR0L = 207;
	
	UCSR0B = (1<<4);  //receptor ativo

	UCSR0C = (1<<5) + (1<<4) + 0x6;  // formato: 8 + 1 stop bit  + paridade impar
	
	while (1) {
	
		if ((UCSR0A & (1<<7)) != 0){
			
			dado = UDR0;
			
			
		}
		
		if (dado == '1'){
				atraso_TIM1_1000Hz();
				PORTB ^= 1 << 7;
			}
			 else if (dado == '2'){
				atraso_TIM1_2000Hz();
				PORTB ^= 1 << 7;
			}
			 else if (dado == '3'){
				atraso_TIM1_4000Hz();
				PORTB ^= 1 << 7;
			}
			 else if (dado == '0'){
				PORTB &=~ (1 << 7);
				OCR1A = 0;
			}
	}
}