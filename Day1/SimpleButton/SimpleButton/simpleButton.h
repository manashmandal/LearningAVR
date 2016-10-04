/*
 * simpleButton.h
 *
 * Created: 10/5/2016 4:14:54 AM
 *  Author: Manash
 */ 


#ifndef SIMPLEBUTTON_H_
#define SIMPLEBUTTON_H_


#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <util/delay.h>
#include "portpins.h"
#include "pinDefines.h"

void simpleButton()
{
	PORTD |= (1 << PD2);
	DDRB = 0xFF;
		
	while(1){
		if (bit_is_clear(PIND, PD2)) {
			PORTB = (1 << PB1);
			} else {
			PORTB = (1 << PB2);
		}
	}
}


#endif /* SIMPLEBUTTON_H_ */