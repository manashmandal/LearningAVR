/*
 * UART_RX_TX.cpp
 *
 * Created: 10/5/2016 8:07:34 PM
 * Author : Manash
 */ 

/*
 * SimpleUART.cpp
 *
 * Created: 10/5/2016 2:41:38 PM
 * Author : Manash
 * CHIP: Test OK on ATmega8
 */ 

// F_CPU must be 8Mhz for 9600 Baud Rate Serial Communication
#define F_CPU 8000000UL

#define BAUD 9600

#define MYUBRR F_CPU/16/BAUD - 1

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

void USART_Init(unsigned int ubrr)
{
	UBRRH = (unsigned char) (ubrr >> 8);
	UBRRL = (unsigned char) ubrr;
	
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << USBS) | (3 <<UCSZ0);
	
}

void USART_Transmit(unsigned char data){
	while (!(UCSRA & (1 << UDRE))) {};
	UDR = data;
}

bool USART_Transmit_String(char* command)
{
	bool ok = false;
	for (int i = 0; i < strlen(command); i++){
		USART_Transmit(command[i]);
		ok = true;
	}
	return ok;
}

unsigned char USART_Receive(void)
{
	while (! (UCSRA & (1 << RXC)));
	return UDR;
}

char *charToString(unsigned char in){
	char *out = (char*) malloc(sizeof(char) * 3);
	out[0] = in;
	out[1] = '\n';
	out[2] = '\0';
	return out;
}


int main(void){
	USART_Init(MYUBRR);
	while(1){
		unsigned char received = USART_Receive();
		USART_Transmit_String(charToString(received));
	}
}

