/*
 * SimpleUART.cpp
 *
 * Created: 10/5/2016 2:41:38 PM
 * Author : Manash
 */ 

// F_CPU must be 8Mhz for 9600 Baud Rate Serial Communication
#define F_CPU 8000000UL

#define BAUD 9600

#define MYUBRR F_CPU/16/BAUD - 1

#include <avr/io.h>
#include <util/delay.h>

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


int main(void){
	USART_Init(MYUBRR);
	while(1){
		USART_Transmit('b');
		_delay_ms(1000);
	}
}

