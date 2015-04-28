/*
 * ADC.h
 *
 * Created: 4/27/2015 2:48:16 PM
 *  Author: shuffleres
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "System.h"
#include "Serial.h"
#include "stdint.h"
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>

char parseString[8];
void calc_ADR(unsigned int value);
void timer0_init();
void ADR_setup();
int ADC_read(byte channel, byte resolution, bool singleEnded);


#endif /* ADC_H_ */