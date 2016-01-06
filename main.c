/*
 * main.c
 *
 *  Created on: 12 gru 2015
 *      Author: Jonasz
 */

#include "microcontroller.h"


void interrupt_init();
void start_measurement(uint16_t timer_compare_value); //w argumencie wartosc do ktorej porownywany jest licznik timera
//void LED_on(char led_port, uint8_t led_pin);
//void LED_off(char led_port, uint8_t led_pin);
void ADC_init();
void IR_init();

int main() {
	DDR(LED0_PORT) |= LED0_PIN;
	//PORT(LED_PORT) |= LED_PIN;
	LED_ON(0);

	while(1) {
		LED_OFF(0);
		_delay_ms(500);
		LED_ON(0);
		_delay_ms(500);
	}

}


void interrupt_init() {
	TCCR1B |= (1 << WGM12);
	TIMSK |= (1 << OCIE1A);
	//OCR1A = 10000; przerwanie z czestotliwoscia 200Hz
}

void ADC_init() {
	ADMUX |= (1<<REFS1) | (1<<REFS0); //ADC internal 2,56 voltage enabled
	ADCSRA |= (1<<ADEN) | (1<<ADIE); //ADC turned on and ADC interrupt enabled
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); //preskaler = 128

	DDR(ADC_PORT) &= ~(ADC0_PIN | ADC1_PIN | ADC2_PIN | ADC3_PIN);
}

void IR_init() {
	DDR(LED0_PORT) |= LED0_PIN;
	DDR(LED1_PORT) |= LED1_PIN;
	DDR(LED2_PORT) |= LED2_PIN;
	DDR(LED3_PORT) |= LED3_PIN;

	PORT(LED0_PORT) &= ~LED0_PIN;
	PORT(LED1_PORT) &= ~LED1_PIN;
	PORT(LED2_PORT) &= ~LED2_PIN;
	PORT(LED3_PORT) &= ~LED3_PIN;
}

void start_measurement(uint16_t timer_compare_value) {
	OCR1A = timer_compare_value;
	led_iterator = 0;

	//TCCR1B |= (1 << CS11); //timer turned on
	//TCCR1B &= ~(1 << CS11); //timer turned off
	switch (led_iterator) {
		case 0: LED_OFF(0); break;
		case 1: LED_OFF(1); break;
		case 2: LED_OFF(2); break;
		case 3: LED_OFF(3); break;
	}
	//pierwszy pomiar
}

ISR(TIMER1_COMPA_vect) {

}

ISR(ADC_vect) {
	measured_values[led_iterator] = 'adcszajs'; //zapisanie napiecia

	switch (led_iterator) {
		case 0: LED_ON(0); break;
		case 1: LED_ON(1); break;
		case 2: LED_ON(2); break;
		case 3: LED_ON(3); break;
	}
	//drugi pomiar
}
