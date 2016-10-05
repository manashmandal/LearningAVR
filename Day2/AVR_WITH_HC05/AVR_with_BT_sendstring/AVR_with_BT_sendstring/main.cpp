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

#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

typedef uint8_t uint;

void USART_Init(unsigned int ubrr)
{
	UBRRH = (unsigned char) (ubrr >> 8);
	UBRRL = (unsigned char) ubrr;
	
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
	
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

bool USART_Transmit_With_CRNL(char *command){
	uint length = strlen(command);
	char *string_with_flags = (char*) malloc(sizeof(char) * (length + 10));
	for (uint i = 0; i < length; i++){
		string_with_flags[i] = command[i];
	}
	string_with_flags[length]     = '\r';
	string_with_flags[length + 1] = '\n';
	string_with_flags[length + 2] = '\0';
	
	USART_Transmit_String(string_with_flags);
	
	free (string_with_flags); //Free the memory 
}

unsigned char USART_Receive(void)
{
	while (! (UCSRA & (1 << RXC)));
	return UDR;
}

uint8_t receiveByte(void){
	loop_until_bit_is_clear(UCSRA, RXC);
	return UDR;
}

char *charToString(unsigned char in){
	char *out = (char*) malloc(sizeof(char) * 4);
	out[0] = in;
	out[1] = '\r';
	out[2] = '\n';
	out[3] = '\0';
	return out;
}



int main(void){
	USART_Init(MYUBRR);
	char* strtosend_ = "Hello AVR Enthusiasts!";
	while(1){
		USART_Transmit_With_CRNL(strtosend_);
		_delay_ms(10);
	}
}

