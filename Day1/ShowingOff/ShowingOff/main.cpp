/*
 * ShowingOff.cpp
 *
 * Created: 10/5/2016 3:08:00 AM
 * Author : Manash
 */ 

//Using ATmega328p

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

#define DELAYTIME 85
#define LED_PORT PORTB
#define LED_DDR DDRB

typedef uint8_t uint;

int main(void)
{
    clock_prescale_set(clock_div_8);
	
	uint i;
	uint repetitions;
	uint whichLED;
	uint16_t randomNumber = 0x1234;
	
	//Inits
	LED_DDR = 0xFF;
	
    while (1) 
    {
		for (i = 0; i < 8; i++){
			LED_PORT |= (1 << i);
			_delay_ms(DELAYTIME);
		}
		
		for (i = 0; i < 8; i++){
			LED_PORT &= ~(1 << i);
			_delay_ms(DELAYTIME);
		}
		
		_delay_ms(DELAYTIME);
		
		for (i = 7; i < 255; i--){
			LED_PORT |= (1 << i);
			_delay_ms(DELAYTIME);
		}
		
		for (i = 7; i< 255; i--){
			LED_PORT &= ~(1 << i);
			_delay_ms(DELAYTIME);
		}
		
		_delay_ms(DELAYTIME * 5);
		
		for (repetitions = 0; repetitions < 75; repetitions++){
			randomNumber = 2053* randomNumber + 13849;
			whichLED = (randomNumber >> 8) & 0b00000111;
			LED_PORT ^= (1 << whichLED);
			_delay_ms(DELAYTIME);
		}
		
		LED_PORT = 0;
		_delay_ms(DELAYTIME * 5);
    }
	
	return (0);
}

