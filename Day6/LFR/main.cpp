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
	int num_active_sensor = 0;
	for (int i = 0; i < NUM_SENSOR; i++){
		if (analogRead(sensors[i]) < threshold){
			sensor_value[i] = ON_LINE;
			num_active_sensor++;
		} else {
			sensor_value[i] = OFF_LINE;
		}
		totalValue += weights[i] * sensor_value[i];
	}
	
	if (num_active_sensor == 0)
		return 0;
	else 
		return (totalValue / num_active_sensor);
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


void followLine(void){
	int position = getPosition();
	USART_Transmit_Number_With_CRNL(position);
}

int main(){
	initPWM();
	motor_init();
	USART_Init(UBRR);
	initADC();
	char x[] = "Hello world\r\n";
	USART_Transmit_String(x);
	while(1){
		
	}
}