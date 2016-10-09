/*
 * ADCMUX.cpp
 *
 * Created: 10/10/2016 1:55:31 AM
 * Author : Manash
 */ 

#include <atmega8/Serial.h>
#include <atmega8/pinDefines.h>

#define THRESHOLD 512

uint16_t analogRead(uint8_t channel){
	ADMUX = (0xf0 & ADMUX) | channel;
	ADCSRA |= (1 << ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);
	return (ADC);
}

void initADC(void)
{
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
}

int main(void)
{
	USART_Init(UBRR);
	initADC();
	LED_DDR = 0xff;
    while (1) 
    {
		uint16_t potValue = analogRead(POT);
		USART_Transmit_Number_With_CRNL(potValue);
		if (potValue > THRESHOLD){
			LED_PORT = 0xff;
		} else {
			LED_PORT = 0x00;
		}
    }
}

