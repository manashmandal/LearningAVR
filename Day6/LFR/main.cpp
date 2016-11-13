#include <atmega8/Serial.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <atmega8/pinDefines.h>
#include <avr/sfr_defs.h>

#define NUM_SENSOR 6
#define ON_LINE 1
#define OFF_LINE 0

#define BASE_SPEED 125
#define DIFF_SPEED 20
#define DELAY_AFTER_STOP 50
#define RETRY 2

unsigned int threshold = 0;

bool pos_changed = false;

int sensors[] = {0, 1, 2, 3, 4, 5};
//int sensors[] = {5, 4, 3, 2, 1, 0};
int weights[] = {100, 200, 300, 1000, 2000, 3000};
	
int analog_value[6];
int sensor_value[6];

int max_value = 0;
int min_value = 0;

int offset = 10;

uint16_t analogRead(uint8_t channel);
	
int getPosition(void){
	int totalValue = 0;
	int num_active_sensor = 0;
	for (int i = 0; i < NUM_SENSOR; i++){
		analog_value[i] = analogRead(sensors[i]);
		if (analog_value[i] < threshold){
			sensor_value[i] = ON_LINE;
			num_active_sensor++;
		} else {
			sensor_value[i] = OFF_LINE;
		}
		totalValue += weights[i] * sensor_value[i];
	}
	
	if (num_active_sensor == 0)
		return 0;
	else if (num_active_sensor == NUM_SENSOR)
		return 10000;
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
	//PORTB &= ~(1 << PB0);
	//PORTD |= (1 << PD7);
}


void rightMotorForward(unsigned int speed){
	OCR1B = speed;
	//PORTD |= (1 << PD6);
	//PORTD &= ~(1 << PD5);
}



void forward(unsigned int ls, unsigned int rs){
	leftMotorForward(ls);
	rightMotorForward(rs);
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
	
	if (position >= 1400 && position < 1500){
		pos_changed = true;
		forward(BASE_SPEED, BASE_SPEED);
	} 
	
	//Right turns
	else if (position >= 1500 && position < 1600){
		pos_changed = true;
		forward(BASE_SPEED + DIFF_SPEED, BASE_SPEED);	
	}
	
	else if (position > 1700 && position < 2200){
		pos_changed = true;
		forward(BASE_SPEED + DIFF_SPEED * 2, BASE_SPEED);	
	}
	
	else if (position > 2200 && position < 2700){
		pos_changed = true;
		forward(BASE_SPEED + 3*DIFF_SPEED, BASE_SPEED - 0.5 * DIFF_SPEED);
	}
	
	else if (position > 2700 && position < 3200){
		pos_changed = true;
		forward(BASE_SPEED + 4 * DIFF_SPEED, BASE_SPEED -  DIFF_SPEED);
	}
	
	//Left turns
	else if (position > 590 && position < 700){
		pos_changed = true;
		forward(BASE_SPEED, BASE_SPEED + DIFF_SPEED);
	}
	
	else if (position > 200 && position < 300){
		pos_changed = true;
		forward(BASE_SPEED, BASE_SPEED + 2* DIFF_SPEED);
	}
	
	else if (position > 120 && position < 170){
		pos_changed = true;
		forward(BASE_SPEED - 0.5 * DIFF_SPEED, BASE_SPEED + 3*DIFF_SPEED);
	}
	
	else if (position > 90 && position < 110){
		pos_changed = true;
		forward(BASE_SPEED -  DIFF_SPEED, BASE_SPEED + 4 * DIFF_SPEED);
	}
	
	else {
		if (pos_changed){
			for (int i = 0; i < RETRY + 1; i++){
				forward(BASE_SPEED, BASE_SPEED);
				_delay_ms(DELAY_AFTER_STOP);
			}
			forward(0, 0);
			pos_changed = false;
		}
		
		else {
			forward(0, 0);
		}
	}
	
}

void ledOn(){
	PORTB |= (1 << PB0);
}

void ledOff(){
	PORTB &= ~(1 << PB0);
}

//Calibrate while blinking
void blink(int times, unsigned int delay, void (*cal)(void)){
	DDRB |= (1 << PB0);
	
	for (int i = 0; i < times; i++) {
		ledOn();
		cal();
		_delay_ms(delay);
		ledOff();
		cal();
		_delay_ms(delay);
	}
	
	ledOn();
	_delay_ms(2000);
	ledOff();
	_delay_ms(1000);
}

void calibrate_max(void){
	max_value = 0;
	getPosition();
	for (int i = 0; i < NUM_SENSOR; i++){
		if (max_value < analog_value[i]) max_value = analog_value[i];
	}
}

void calibrate_min(void){
	min_value = 0;
	getPosition();
	for (int i = 0; i < NUM_SENSOR; i++){
		if (min_value > analog_value[i]) min_value = analog_value[i];
	}
}

void calibrate(void){
	threshold = (max_value + min_value) / 2;
}



int main(){
	initPWM();
	motor_init();
	USART_Init(UBRR);
	initADC();
	calibrate();
	char x[] = "Hello world\r\n";
	USART_Transmit_String(x);
	
	blink(5, 300, calibrate_max);
	blink(4, 150, calibrate_min);
	
	calibrate();
	
	USART_Transmit_Number_With_CRNL(threshold);
	
	while(1){
		followLine();
	}
}
