/*
 * FreeRunningADC.cpp
 *
 * Created: 10/10/2016 1:29:26 AM
 * Author : Manash
 */ 

#include <atmega8/Serial.h>
#include <atmega8/pinDefines.h>

#define SAMPLE_DELAY 20

static inline void initFreeRunningADC(void)
{
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
	
	ADMUX |= (1 << ADLAR);
	
	ADCSRA |= (1 << ADEN);
	
	//For atmega8 using ADFR instead of ADATE [http://www.avrfreaks.net/forum/what-difference-between-adate-and-adfr]
	ADCSRA |= (1 << ADFR);
	
	ADCSRA |= (1 << ADSC);
}

int main(void)
{
	USART_Init(UBRR);
	initFreeRunningADC();
	
    /* Replace with your application code */
    while (true) 
    {
		USART_Transmit_Number_With_CRNL(ADCH);
		_delay_ms(100);
    }
}

