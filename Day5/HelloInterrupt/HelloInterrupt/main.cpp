/*
 * HelloInterrupt.cpp
 *
 * Created: 10/10/2016 11:51:34 PM
 * Author : Manash
 */ 

#include <atmega8/Serial.h>
#include <atmega8/pinDefines.h>
#include <avr/interrupt.h>

ISR(INT0_vect)
{
	//Proof that interrupt was really triggered
	char *interrupt_triggered = "Interrupt triggered!";
	USART_Transmit_With_CRNL(interrupt_triggered);
	
	if (bit_is_set(BUTTON_PIN, BUTTON)){
		LED_PORT |= (1 << LED1);
	} else {
		LED_PORT &= ~(1 << LED1);
	}
}

void initInterrupt0(void)
{
	MCUCR |= (1 << ISC00); // For atmega328p EIMSK
	GICR |= (1 << INT0); // For atmega328p ISC00
	sei();
}

int main(void)
{
    LED_DDR = 0xff;
	BUTTON_PORT |= (1 << BUTTON);
	USART_Init(UBRR);
	initInterrupt0();
    while (1) 
    {
		_delay_ms(100);
		LED_PORT ^= (1 << LED0);
    }
}

