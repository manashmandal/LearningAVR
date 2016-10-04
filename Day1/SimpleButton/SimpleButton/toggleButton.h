/*
 * toggleButton.h
 *
 * Created: 10/5/2016 4:16:17 AM
 *  Author: Manash
 */ 


#ifndef TOGGLEBUTTON_H_
#define TOGGLEBUTTON_H_

#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <util/delay.h>
#include "portpins.h"
#include "pinDefines.h"

void toggleButton()
{
	bool buttonWasPressed;
	BUTTON_PORT |= (1 << BUTTON);
	LED_DDR = (1 << LED0);
		
	while(1){
		if (bit_is_clear(BUTTON_PIN, BUTTON)){
			if (!buttonWasPressed){
				LED_PORT ^= (1 << LED0);
				buttonWasPressed = true;
			}
			} else {
			buttonWasPressed = false;
		}
	}
}


#endif /* TOGGLEBUTTON_H_ */