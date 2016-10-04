/*
 * Blinky.cpp
 *
 * Created: 10/5/2016 1:32:15 AM
 * Author : Manash
 */ 

#define F_CPU 16000000UL

#define loop while(true)

#include <avr/io.h>
#include <util/delay.h>

#define OUTPUT 1
#define INPUT 0

#define HIGH 1
#define LOW 0

typedef volatile unsigned char vuc;


void pinMode(vuc &DDR, bool mode)
{
	if (mode){
		DDR = 0xFF;
		return;
	}
	DDR = 0x00;
}

void digitalWrite(vuc &PORT, bool mode)
{
	if (mode){
		PORT = 0xFF;
		return;
	}
	PORT = 0x00;
}

void blink(vuc &pin, unsigned int delay_time = 100)
{
	digitalWrite(pin, HIGH);
	_delay_ms(delay_time);
	digitalWrite(pin, LOW);
	_delay_ms(delay_time);
}

int main(void)
{
	pinMode(DDRD, true);
	
	loop {
		blink(PORTD);
	}
}

