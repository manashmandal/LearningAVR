/*
 * POTReader.cpp
 *
 * Created: 10/7/2016 12:34:36 AM
 * Author : Manash
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <string.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include "pinDefines.h"

#define BAUD 9600

#define MYUBRR F_CPU/16/BAUD - 1


typedef uint8_t uint;

char *string_read = (char*) malloc(sizeof(char) * 100);

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

void readString(void){
	string_read[0] = '\0';
	uint i = 0;
	uint maxlen = 50;
	char response;
	while (i < (maxlen - 1)){
		response = USART_Receive();
		if (response == '\r' || response == '\n'){
			break;
			} else {
			string_read[i] = response;
			i++;
		}
	}
	string_read[i] = '\0';
	USART_Transmit_With_CRNL(string_read);
}

static inline void initADC0(void){
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
}

int main(void)
{
	//Inits
	USART_Init(MYUBRR);
	uint16_t adcValue;
	uint8_t i;
	initADC0();
	LED_DDR	= 0xFF;
	
    /* Replace with your application code */
    while (1) 
    {
		
		ADCSRA |= (1 << ADSC);
		loop_until_bit_is_clear(ADCSRA, ADSC);
		
		adcValue = ADC;
		
		
		char str[10];
		
		//Converts number to String
		sprintf(str, "%d", adcValue);
		
		if (adcValue > 512) LED_PORT |= (1 << LED1);
		else LED_PORT &= ~(1 << LED1);
		
		// Sending the value via UART
		USART_Transmit_With_CRNL(str);

		_delay_ms(100);
    }
	return (0);
}

