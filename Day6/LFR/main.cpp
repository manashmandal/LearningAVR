/*
 * LFR.cpp
 *
 * Created: 11/10/2016 2:07:21 PM
 * Author : Manash
 */ 


//#define F_CPU 16000000UL

//#define BAUD 9600

//#define MYUBRR F_CPU/16/BAUD - 1

#include <atmega8/Serial.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <atmega8/pinDefines.h>
#include <avr/sfr_defs.h>

#define NUM_SENSOR 6
#define ON_LINE 1
#define OFF_LINE 0

int threshold = 400;

//int sensors[] = {0, 1, 2, 3, 4, 5};
int sensors[] = {5, 4, 3, 2, 1, 0};
int weights[] = {1000, 2000, 3000, 4000, 5000, 6000};
	
int sensor_value[6];

uint16_t analogRead(uint8_t channel);
	
int getPosition(void){
	int totalValue = 0;
	for (int i = 0; i < NUM_SENSOR; i++){
		if (analogRead(sensors[i]) < threshold){
			sensor_value[i] = ON_LINE;
		} else {
			sensor_value[i] = OFF_LINE;
		}
		totalValue += weights[i] * sensor_value[i];
	}
	return (totalValue / NUM_SENSOR);
}

void debugSensor(int pos, int val){
	char str[50];
	sprintf(str, "IR [%d] = %d", pos, val);
	USART_Transmit_With_CRNL(str);
}

void motor_init(void){
	DDRB |= (1 << PB0);
	DDRD |= (1 << PD7);
}

uint16_t analogRead(uint8_t channel){
	ADMUX = (0xF0 & ADMUX) | channel;
	ADCSRA |= (1 << ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);
	return (ADC);
}

void initADC(void){
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
}


void leftMotorForward(unsigned int speed){
	OCR1A = speed;
	PORTB &= ~(1 << PB0);
	PORTD |= (1 << PD7);
}

void leftMotorBackward(unsigned int speed){
	OCR1A = speed;
	PORTB |= (1 << PB0);
	PORTD &= ~(1 << PD7);
}

void rightMotorForward(unsigned int speed){
	OCR1B = speed;
	PORTD |= (1 << PD6);
	PORTD &= ~(1 << PD5);
}

void rightMotorBackward(unsigned int speed){
	OCR1B = speed;
	PORTD &= ~(1 << PD6);
	PORTD |= (1 << PD5);
}


void forward(unsigned int ls, unsigned int rs){
	leftMotorForward(ls);
	rightMotorForward(rs);
}

void backward(unsigned int ls, unsigned int rs){
	leftMotorBackward(ls);
	rightMotorBackward(rs);
}

/*
void USART_Init(unsigned int ubrr)
{
	UBRRH = (unsigned char) (ubrr >> 8);
	UBRRL = (unsigned char) ubrr;
	
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << USBS) | (3 <<UCSZ0);
	
}

void USART_Transmit(unsigned char data){
	while (!(UCSRA & (1 << UDRE))) {};
	UDR = data;
}

bool USART_Transmit_String(char* command)
{
	bool ok = false;
	for (int i = 0; i < strlen(command); i++){
		USART_Transmit(command[i]);
		ok = true;
	}
	return ok;
}
*/

static inline void initPWM(void){
	// Selecting PWM Mode [FastPWM - 8bit]
		// Selecting PWM Mode [FastPWM - 8bit]
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS11);
	TCCR1A |= (1 << COM1A1);
	TCCR1A |= (1 << COM1B1);
	
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
}


/*
int main(void){
	USART_Init(MYUBRR);
	char command[] = "Hello World!\r\n";
	while(1){
		USART_Transmit_String(command);
		_delay_ms(1000);
	}
}
*/


/*
int main(void){
	initAllOut();
	while(true){
		blinkAll();
	}
}*/

/*
int main(void){
	initPWM();
	
	while(true){
		for (int i = 0; i < 255; i++){
			OCR1A = i;
			OCR1B = i;
			_delay_ms(10);
		}
		_delay_ms(1000);
	}
}
*/

void followLine(void){
	int position = getPosition();
	USART_Transmit_Number_With_CRNL(position);
	
	////HARD RIGHT
	//if (position > 100 && position < 200){
		//forward(180, 165);
	//} 
	//// SOFT RIGHT
	//else if (position > 200 && position <= 500){
		//forward(170, 165);
	//}
	//
	//// HARD LEFT
	//else if (position >= 2000 && position < 3000){
		//forward(165, 180);
	//}
	//
	//// SOFT LEFT
	//else if (position > 1700 && position < 2000) {
		//forward(165, 170);
	//}
	//
	//// FORWARD
	//else if (position >= 1000 && position <= 1700){
		//forward(165, 165);
	//}
	//
	//else {
		//forward(0, 0);
	//}
	
	if (position >= 1000 && position <= 1666){
		forward(150, 150);
	} else {
		forward(0, 0);
	}
}

int main(){
	initPWM();
	motor_init();
	USART_Init(UBRR);
	initADC();
	char x[] = "Hello world\r\n";
	USART_Transmit_String(x);
	while(1){
		//forward(150, 165);
	USART_Transmit_With_CRNL("----------\n\n");
	for (int i = 0; i < NUM_SENSOR; i++){
		debugSensor(i, analogRead(sensors[i]));
		_delay_ms(100);	
	}
		//debugSensor(0, getPosition());
		//followLine();
		//USART_Transmit_With_CRNL("----------\n\n");
		//USART_Transmit_Number_With_CRNL(analogRead(6));
		_delay_ms(1000);
		
		
	}
}
