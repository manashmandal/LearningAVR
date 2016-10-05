/*
 * SendDataToAVRViaBluetooth.cpp
 *
 * Created: 10/6/2016 2:13:28 AM
 * Author : Manash
 * Chip: ATMEGA328P
 */ 


/*

SELF NOTE:
FOR ATMEGA8A with Bluetooth Module, F_CPU = 8Mhz works flawlessly with Baudrate 9600
FOR ATMEGA8A with Bluetooth Module, F_CPU = 16Mhz works flawlessly with Baudrate 19200

FOR ATMEGA328P with Bluetooth Module, F_CPU = 16Mhz works flawlessly with Baudrate 9600

*/

// F_CPU must be 8Mhz for 9600 Baud Rate Serial Communication
#define F_CPU 16000000UL

#define BAUD 9600

#define MYUBRR F_CPU/16/BAUD - 1

#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>


typedef uint8_t uint;

char *string_read = (char*) malloc(sizeof(char) * 100);

void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void USART_Transmit(unsigned char data){
	while (!(UCSR0A & (1 << UDRE0))) {};
	UDR0 = data;
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
	while (! (UCSR0A & (1 << RXC0)));
	return UDR0;
}

uint8_t receiveByte(void){
	loop_until_bit_is_clear(UCSR0A, RXC0);
	return UDR0;
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

int main(void){
	USART_Init(MYUBRR);
	while(1){
		readString();
		_delay_ms(10);
	}
}



