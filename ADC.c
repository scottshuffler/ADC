/*
 * ADC.c
 *
 * Created: 4/21/2015 4:27:10 PM
 *  Author: shuffleres
 */ 
#include "ADC.h"

volatile uint8_t overflow;
volatile int avg_Read;

// Set up Serial 19200, and 8N1
// Turn on timer / counter 0
// and start TCNT0 counter
// Lets TCNT0 overflow 4 times
// After 4 overflows waits for TCNT0 to count to 226
// This ensure there will be a 20ms pause between ADC polls
int main(void)
{
	serial_open(19200, SERIAL_8N1);
	cli();
	timer0_init();
	sei();
	ADR_setup();
    while(1)
    {
		if (tot_overflow >= 4)
		{
			if (TCNT0 >=226)
			{
				avg_Read = ADC_read(0, 10, true);
				unsigned int value = avg_Read;
				calc_ADR(value);
				TCNT0 = 0;
				tot_overflow = 0;
				avg_Read = 0;
			}
		}
    }
}

// Sets up the ADR to be polled
void ADR_setup()
{
	ADMUX |= (1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// Sets up timer for 256 prescale
// Makes TNCT0 = 0
// And enables overflow interrupts
void timer0_init()
{
	TCCR0B |= (1 << CS02);
	TCNT0 = 0;
	TIMSK0 |= (1 << TOIE0);
	tot_overflow = 0;
}

// Gets the snapshot value of the fifth poll
// Uses this value to print out what needs to be displayed
void calc_ADR(unsigned int value)
{
	double newValue = (double)value * 5.0 / 1023.0;
	dtostrf(newValue, 4, 2, parseString);
	prints(parseString);
	sprintf(parseString,"0x%X",value);
	prints(" volts (");
	prints(parseString);
	prints(")\n");
}

// Reads ADC value
// Sets ADC conversion bit
// Waits for ADC Interrupt to updated
//Clears interrupt flag
int ADC_read(byte channel, byte res, bool singleEnded)
{
	ADCSRA |=(1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	return ADC;
}

// ISR overflow interrupt handler 
// Adds 1 to the overflow counter each time
// Only reads if the overflow is less than or equal to 4
ISR(TIMER0_OVF_vect)
{
	overflow = overflow + 1;
	if (overflow <= 4)
	{
		avg_Read = ADC_read(0, 10, true);
	}
}