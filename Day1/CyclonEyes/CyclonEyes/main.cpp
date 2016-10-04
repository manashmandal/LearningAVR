/*
 * CyclonEyes.cpp
 *
 * Created: 10/5/2016 1:41:23 AM
 * Author : Manash
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define DELAYTIME 85
#define LED_PORT PORTB
#define LED_PIN PINB
#define LED_DDR DDRB

typedef uint8_t uint;

int main(void)
{
    uint i;
	LED_DDR = 0xFF;
	
    while (1) 
    {
		while(i < 7){
			LED_PORT = (1 << i);
			_delay_ms(DELAYTIME);
			i++;
		}
		
		while (i > 0){
			LED_PORT = (1 << i);
			_delay_ms(DELAYTIME);
			i--;
		}
    }
	
	return (0);
}

