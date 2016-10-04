/*
 * SimpleButton.cpp
 *
 * Created: 10/5/2016 3:41:44 AM
 * Author : Manash
 */ 

#include "simpleButton.h"
#include "toggleButton.h"
#include "debounceButton.h"

#define DEBOUNCE_TIME 1000

//simpleButton -> simpleButton()
// toggleButton -> toggleButton()
// debounceButton -> debounceButton()

// LED -> PB2
// Button -> PD2

int main(void)
{
	while(true){
		//simpleButton();
		//toggleButton();
		debounceButton();
	}
}
