/*
 * bluetooth.h
 *
 *  Created on: 15 gru 2015
 *      Author: Jonasz
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define USARTBufferSize 32

void initUSART();
void sendMessage(char *message);

volatile uint8_t receivedMessage;
volatile uint16_t message;
volatile uint8_t messageStarted;
volatile uint8_t messageEnded;
volatile uint8_t wasMessageReceived;
volatile uint8_t USARTBufferIndex;
char USARTBuffer[USARTBufferSize];

#endif /* BLUETOOTH_H_ */
