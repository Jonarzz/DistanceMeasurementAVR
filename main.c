/*
 * main.c
 *
 *  Created on: 12 gru 2015
 *      Author: Jonasz
 */

#include "distanceMeasurement.h"

int main() {

	IR_init();
	interrupt_init();
	ADC_init();
	sei();

	//start_measurement(1);

	while(1) {

//		if (measurementDataComplete) {
//			start_measurement(1);
//		}
	}

}

ISR(TIMER1_COMPA_vect) {
	TCCR1B &= ~(1 << CS11); //wylaczenie timera
	//wybor pinu ADC
	ADMUX &= 0b11100000; //maskowanie 3 najstarszych bitow
	ADMUX |= led_iterator; //wlaczenie ADC(led_iterator)
	ADCSRA |= (1<<ADSC); //wlaczenie pojedynczego pomiaru po ktorym wystapi przerwanie
}

ISR(ADC_vect) {
	switch (led_iterator) {
		case 0: LED_OFF(0); break;
		case 1: LED_OFF(1); break;
		case 2: LED_OFF(2); break;
		case 3: LED_OFF(3); break;
	}

	if (!wasMeasuredWithLedOn) {
		measured_values[led_iterator] = - ADC; //pomiar z wylaczona dioda zebrany ze znakiem minus (zostanie on odjety od pomiaru z zapalona dioda)
		start_measurement(diodeShiningTime);
	}
	else {
		measured_values[led_iterator] += ADC; //pomiar z wycietym wplywem otoczenia
		led_iterator++;
		wasMeasuredWithLedOn = 0;
		if (led_iterator < NUM_OF_IRS) {
			start_measurement(1);
		}
		else {
			led_iterator = 0; //koniec pomiaru odleglosci po 8 pomiarach
			measurementDataComplete = 1;
		}
	}
}

