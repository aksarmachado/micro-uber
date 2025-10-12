/*
 * Protocolo.c
 *
 * Created: 11/10/2025 19:22:17
 *  Author: Aksa
 */ 
// teste de comunica����o serial entre o microcontrolador e o servidor externo

// mensagens de comunica��o entre servidor e microcontrolador 

//Fun��o para transmitir a mensagem de qual o movimento do veiculo por comunica��o serial para o servidor externo
void Transm_mov_veiculo(char mov){
	char envia[4]={0};		//Vetor que receber� a mensagem de qual movimento o carro ter�
	
	envia[0]='U';			//Mensagem para ser transmitida indicando movimenta��o
	envia[1]='M';
	envia[2]= mov;			//Indica qual o movimento do carro, sendo 0-parado, 1-move p/ frente, 2-vira p/ direita, 3-vira p/ esquerda
	envia[3]='\0';
	
	transmite_string_serial(envia, 3);//transmite mensagem de qual o movimento o carro ter� pela serial
	
}

//Fun��o para transmitir a mensagem de aceite do cliente por comunica��o serial para o servidor externo(recebe o byte contendo o codigo do cliente)
void Transm_aceita_cliente(char cliente){
	char envia[4]={0};		//Vetor que receber� a mensagem de qual movimento o carro ter�
	
	envia[0]='U';			//Mensagem para ser transmitida indicando aceite de cliente
	envia[1]='A';
	envia[2]= cliente;		//Aloca  conteudo do byte do cliente no terceiro byte da mensagem a ser transmitida
	envia[3]='\0';
	
	
	
	
	transmite_string_serial(envia, 3); //transmite mensagem de aceite do cliente para o servidor externo
	//escreve_string_lcd("SA");			// mensagem para sinalizar o cliente aceito no LCD
	//_delay_ms(200);
	//limpa_display();
	
	