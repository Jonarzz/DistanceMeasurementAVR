/*
 * bluetooth.c
 *
 *  Created on: 15 gru 2015
 *      Author: Jonasz
 */

#include "bluetooth.h"
#include <string.h>

void initUSART() {
	#define BAUD 9600
	#define CLOCK_FREQ 16000000
	#include <util/setbaud.h>
	#define UBRR ((CLOCK_FREQ/16/BAUD) - 1)

	UBRR0H = UBRR>>8;
	UBRR0L = UBRR;

	UCSR0B = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);
	UCSR0C = (1<<UCSZ1) | (1<<UCSZ0);
}

void sendMessage(char message[USARTBufferSize - 2]) {
	strncpy(USARTBuffer, message, USARTBufferSize - 2);
	unsigned char z;
	for (z = 0; z < USARTBufferSize - 2; z++) {
		if (USARTBuffer[z] == 0) {   //czy to koniec takstu w tablicy
			USARTBuffer[z] = 13;  //znak powrotu karetki CR (Carrige Return)
			USARTBuffer[z + 1] = 10; //znak nowej linii LF (Line Feed)
			USARTBuffer[z + 2] = 0;  //znak koñca ci¹gu tekstu w tablicy
			break;
		}
	}

	while (!(UCSR0A & (1<<UDRE)));

	USARTBufferIndex = 0;
	UCSR0B |= (1<<UDRIE);
}
