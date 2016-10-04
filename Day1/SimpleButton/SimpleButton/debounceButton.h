/*
 * debounceButton.h
 *
 * Created: 10/5/2016 4:17:44 AM
 *  Author: Manash
 */ 


#ifndef DEBOUNCEBUTTON_H_
#define DEBOUNCEBUTTON_H_

#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <util/delay.h>
#include "portpins.h"
#include "pinDefines.h"

#define DEBOUNCE_TIME 1000

bool debounce(void){
	if (bit_is_clear(BUTTON_PIN, BUTTON)){
		_delay_us(DEBOUNCE_TIME);
		if (bit_is_clear(BUTTON_PIN, BUTTON)){
			return true;
		}
	}
	return false;
}

void debounceButton(void)
{
	bool buttonWasPressed;
	BUTTON_PORT |= (1 << BUTTON);
	LED_DDR = (1 << LED0);
		
	while(true){
		if (debounce()){
			if (!buttonWasPressed){
				LED_PORT ^= (1 << LED0);
				buttonWasPressed = true;
			}
			} else {
			buttonWasPressed = false;
		}
	}
}


#endif /* DEBOUNCEBUTTON_H_ */