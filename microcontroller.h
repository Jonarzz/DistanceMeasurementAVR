/*
 * microcontroller.h
 *
 *  Created on: 13 gru 2015
 *      Author: Jonasz
 */

#ifndef MICROCONTROLLER_H_
#define MICROCONTROLLER_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


// *** DDR - setting in/out on the pin
#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)
// *** PORT - setting logical high/low state on the pin
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)
// *** PIN - reading logical high/low state on the pin
#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)
// *** P - definition of the pin and the port (e.g. PB2, PC4)
#define P(x, y) XP(x, y)
#define XP(x, y) (P##x##y)

#define LED_PORT(x) XLED_PORT(x)
#define XLED_PORT(x) LED##x##_PORT

#define LED_PIN(x) XLED_PIN(x)
#define XLED_PIN(x) LED##x##_PIN


#define LED0_PORT C
#define LED0_PIN 1<<P(LED0_PORT, 0)

#define LED1_PORT C
#define LED1_PIN 1<<P(LED1_PORT, 1)

#define LED2_PORT C
#define LED2_PIN 1<<P(LED2_PORT, 2)

#define LED3_PORT C
#define LED3_PIN 1<<P(LED3_PORT, 3)

#define LED_ON(x) PORT(LED_PORT(x)) |= LED_PIN(x);
#define LED_OFF(x) PORT(LED_PORT(x)) &= ~(LED_PIN(x));

#define ADC_PORT F
#define ADC0_PIN 1<<P(ADC_PORT, 0)
#define ADC1_PIN 1<<P(ADC_PORT, 1)
#define ADC2_PIN 1<<P(ADC_PORT, 2)
#define ADC3_PIN 1<<P(ADC_PORT, 3)

#define NUM_OF_IRS 4

uint8_t led_iterator;
uint16_t measured_values [NUM_OF_IRS];



#endif /* MICROCONTROLLER_H_ */
