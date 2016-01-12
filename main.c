/*
 * main.c
 *
 *  Created on: 12 gru 2015
 *      Author: Jonasz
 */

#include "distanceMeasurement.h"
#include "bluetooth.h"

int main() {

	initUSART();
	IR_init();
	interrupt_init();
	ADC_init();
	sei();

	start_measurement(diodeOffTime);

	while(1) {
		_delay_ms(100);

		if (measurementDataComplete) {

			for (uint8_t i = 0; i < NUM_OF_IRS; i++) {
				itoa(measured_values[i], dataToSend , 10);
				sendMessage(dataToSend);
				_delay_ms(10);
			}
			sendMessage(" ");


			start_measurement(diodeOffTime);
		}


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
		measured_values[led_iterator] = -ADC; //pomiar z wylaczona dioda zebrany ze znakiem minus (zostanie on odjety od pomiaru z zapalona dioda)
		start_measurement(diodeShiningTime);
	}
	else {
		measured_values[led_iterator] += ADC; //pomiar z wycietym wplywem otoczenia
		led_iterator++;
		wasMeasuredWithLedOn = 0;
		if (led_iterator < NUM_OF_IRS) {
			start_measurement(diodeOffTime);
		}
		else {
			led_iterator = 0; //koniec pomiaru odleglosci po 8 pomiarach
			measurementDataComplete = 1;
		}
	}
}

ISR(USART0_RX_vect) {
	receivedMessage = UDR0;   //zapamiêtaj odebran¹ liczbê
	if (messageStarted == 0 && receivedMessage == '^') {
		message = 0;
		messageStarted = 1;
	}

	if (messageStarted && receivedMessage != '^' && receivedMessage != '&') {
		receivedMessage -= '0';
		if (message == 0) {
			message += receivedMessage;
		} else if (message < 10) {
			message = message*10 + receivedMessage;
		} else if (message < 100) {
			message = message*10 + receivedMessage;
		} else if (message < 1000) {
			message = message*10 + receivedMessage;
		}
	}

	if (messageStarted == 1 && receivedMessage == '&') {
		messageStarted = 0;
		messageEnded = 1;
	}

	wasMessageReceived = 1; //ustaw flagê odbioru liczby dla main()
}

ISR(USART0_UDRE_vect){
	 if(USARTBuffer[USARTBufferIndex] != 0) {
		 UDR0 = USARTBuffer[USARTBufferIndex++];
	 }
	 else {
		 UCSR0B &= ~(1<<UDRIE);
	 }
}
