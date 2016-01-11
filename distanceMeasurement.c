/*
 * microcontroller.c
 *
 *  Created on: 13 gru 2015
 *      Author: Jonasz
 */

#include "distanceMeasurement.h"

volatile uint8_t led_iterator = 0;
volatile uint8_t wasMeasuredWithLedOn = 0;
volatile uint8_t measurementDataComplete = 0;
const uint16_t diodeShiningTime = 50;
const uint16_t diodeOffTime = 1;

void interrupt_init() {
	TCCR1B |= (1 << WGM12);
	TIMSK |= (1 << OCIE1A);
	//OCR1A = 10000; przerwanie z czestotliwoscia 200Hz
}

void ADC_init() {
	ADMUX |= (1<<REFS1) | (1<<REFS0); //Podpiete wewnetrzne zrodlo 2,56 dla przetwornika ADC
	ADCSRA |= (1<<ADEN) | (1<<ADIE); //ADC wlaczony, przerwania na zakonczenie pomiaru wlaczone
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
	measurementDataComplete = 0; //zerujemy te zmienna aby zablokowac wywolywanie sie pomiarow przed zakonczeniem poprzednich
	OCR1A = timer_compare_value * 2; //OCR1A = (timer_compare_value * 2000000Hz / 1000000) obliczona gorna granica licznika z danego czasu na przerwanie

	if (timer_compare_value > diodeOffTime) {
		switch (led_iterator) {
			case 0: LED_ON(0); break;
			case 1: LED_ON(1); break;
			case 2: LED_ON(2); break;
			case 3: LED_ON(3); break;
		}
		wasMeasuredWithLedOn = 1;
	}

	TCCR1B |= (1 << CS11); //timer wlaczony
}

