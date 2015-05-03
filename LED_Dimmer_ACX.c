/*
 * ADC.c
 *
 * Created: 4/21/2015 4:27:10 PM
 *  Author: shuffleres
 */ 
#include "LED_Dimmer_ACX.h"

volatile uint8_t overflow;
volatile int avg_Read;
char parseString[8];


void duty();
void read();
void blink();
void send();
int ADC_read();
void ADR_setup();
void calc_ADR();
volatile int adc_readings = 0;

int main(void)
{
	serial_open(19200, SERIAL_8N1);
	//ADR_setup();
	pinMode(52, OUTPUT);
	pinMode(53, OUTPUT);
	pinMode(13, OUTPUT);
	digitalWrite(52,HIGH);
	digitalWrite(53,HIGH);
	/*x_init();
	x_new(1, dimmer, 1);
	x_new(2, setDelay, 1);
	x_new(3, blinker, 1);
	*/
	sei();
	ADR_setup();
	x_init();
	x_new(1, read, 1);
	x_new(2, duty, 1);
	x_new(3, send, 1);
	x_new(4, blink, 1);
	x_yield();
	while (1)
	{
		x_delay(500);
	}
}

//Thread 1
void read()
{
	while (1)
	{
		adc_readings = ADC_read();
		x_delay(500);
		x_yield();
	}
}

//Thread 2
void duty()
{
	while (1)
	{
		digitalWrite(53, HIGH);
		x_delay((adc_readings >> 5) + 1);
		digitalWrite(53, LOW);
		x_delay(31 - (adc_readings >> 5) + 1);
		x_yield();
	}
}

//Thread 3
void send()
{
	while(1)
	{
		calc_ADR(adc_readings);
		x_delay(500);
		x_yield();
	}
}

//Thread 4
void blink()
{
	while(1)
	{
		digitalWrite(52, HIGH);
		x_delay(125);
		digitalWrite(52, LOW);
		x_delay(125);
		x_yield();
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
	overflow = 0;
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
int ADC_read()
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

void pinMode(int pin, char mode)
{
	if (pin > 53 || pin < 0)
	{
		return;
	}
	if (mode < 0 || mode > 2)
	{
		return;
	}
	//Input
	if (mode == 0)
	{
		*(map[pin].ddr) &= ~(1 << map[pin].bitShift);
		*(map[pin].ddr + 1) &= ~(1 << map[pin].bitShift);
	}
	//Output
	else if (mode == 1)
	{
		*(map[pin].ddr) |= (1 << map[pin].bitShift);
	}
	//Pullup
	else if(mode == 2)
	{
		*(map[pin].ddr) &= ~(1 << map[pin].bitShift);
		*(map[pin].ddr + 1) |= (1 << map[pin].bitShift);
	}
	
}

//Tests to see if there is a voltage on the pin
int digitalRead(int pin)
{
	return (*(map[pin].ddr - 1) & (1 << map[pin].bitShift)) ? 1 : 0;

}

//Changes the bit to turn it on or off
void digitalWrite(int pin, int value)
{
	if (value < 0 || value > 1)
	{
		return;
	}
	if (value == 1)
	{
		*(map[pin].ddr + 1) |= (1 << map[pin].bitShift);
	}
	else
	{
		*(map[pin].ddr + 1) &= ~(1 << map[pin].bitShift);
	}
}

void serial_open(long speed, int config)
{
	//Switch for baud rate
	//Uses table from the data sheet
	//Sets U2X0 to 1 or 0 depending on which has a lower % error
	switch (speed)
	{
		case 2400:
		UBRR0 = 832;
		UCSR0A |= (1<<U2X0);
		break;
		case 4800:
		UBRR0 = 207;
		UCSR0A &= ~(1<<U2X0);
		break;
		case 9600:
		UBRR0 = 103;
		UCSR0A &= ~(1<<U2X0);
		break;
		case 14400:
		UBRR0 = 68;
		UCSR0A &= ~(1<<U2X0);
		break;
		case 19200:
		UBRR0 = 51;
		UCSR0A &= ~(1<<U2X0);
		break;
		case 28800:
		UBRR0 = 68;
		UCSR0A |= (1<<U2X0);
		break;
		case 38400:
		UBRR0 = 25;
		UCSR0A &= ~(1<<U2X0);
		break;
		case 57600:
		UBRR0 = 16;
		UCSR0A &= ~(1<<U2X0);
		break;
		case 76800:
		UBRR0 = 12;
		UCSR0A &= ~(1<<U2X0);
		break;
		case 115200:
		UBRR0 = 16;
		UCSR0A |= (1<<U2X0);
		break;
		case 230400:
		UBRR0 = 3;
		UCSR0A &= ~(1<<U2X0);
		break;
		case 250000:
		UBRR0 = 3;
		UCSR0A &= ~(1<<U2X0);
		break;
		default:
		return;
	}
	
	//Switch for the configuration
	//Defaults to 8N1 if a valid choice was not entered
	//Sets certain bits on or off
	//                      UCSR0C
	//   7       6      5     4     3      2      1     0
	//UMSEL01-UMSEL00-UPM01-UPM00-USBS0-UCSZ01-UCSZ00-UCPOL0
	// Synch   Synch   Par   Par  Stop   Data   Data  Data
	switch (config)
	{
		case SERIAL_5N1:
		UCSR0C = UCSR0C & 00000000;
		break;
		case SERIAL_6N1:
		UCSR0C |= (1<<UCPOL0);
		break;
		case SERIAL_7N1:
		UCSR0C |= (1<<UCSZ00);
		break;
		case SERIAL_8N1:
		UCSR0C &= (3<<UCSZ00);
		break;
		case SERIAL_5N2:
		UCSR0C |= (1<<USBS0);
		break;
		case SERIAL_6N2:
		UCSR0C |= (1<<USBS0) | (1<<UCPOL0);
		break;
		case SERIAL_7N2:
		UCSR0C |= (1<<USBS0) | (1<<UCSZ00);
		break;
		case SERIAL_8N2:
		UCSR0C |= (1<<USBS0) | (3<<UCSZ00);
		break;
		case SERIAL_5E1:
		UCSR0C |= (1<<UMSEL00);
		break;
		case SERIAL_6E1:
		UCSR0C |= (1<<UMSEL00) | (1<<UCPOL0);
		break;
		case SERIAL_7E1:
		UCSR0C |= (1<<UMSEL00) | (1<<UCSZ00);
		break;
		case SERIAL_8E1:
		UCSR0C |= (1<<UMSEL00) | (3<<UCSZ00);
		break;
		case SERIAL_5E2:
		UCSR0C |= (1<<UMSEL00) | (1<<USBS0);
		break;
		case SERIAL_6E2:
		UCSR0C |= (1<<UMSEL00) | (1<<USBS0) | (1<<UCPOL0);
		break;
		case SERIAL_7E2:
		UCSR0C |= (1<<UMSEL00) | (1<<USBS0) | (1<<UCSZ00);
		break;
		case SERIAL_8E2:
		UCSR0C |= (1<<UMSEL00) | (1<<USBS0) | (3<<UCSZ00);
		break;
		case SERIAL_5O1:
		UCSR0C |= (3<<UMSEL00);
		break;
		case SERIAL_6O1:
		UCSR0C |= (3<<UMSEL00) | (1<<UCPOL0);
		break;
		case SERIAL_7O1:
		UCSR0C |= (3<<UMSEL00) | (1<<UCSZ00);
		break;
		case SERIAL_8O1:
		UCSR0C |= (3<<UMSEL00) | (3<<UCSZ00);
		break;
		case SERIAL_5O2:
		UCSR0C |= (3<<UMSEL00) | (1<<USBS0);
		break;
		case SERIAL_6O2:
		UCSR0C |= (3<<UMSEL00) | (1<<USBS0) | (1<<UCPOL0);
		break;
		case SERIAL_7O2:
		UCSR0C |= (3<<UMSEL00) | (1<<USBS0) | (1<<UCSZ00);
		break;
		case SERIAL_8O2:
		UCSR0C |= (3<<UMSEL00) | (1<<USBS0) | (3<<UCSZ00);
		break;
		default:
		UCSR0C &= (3<<UCSZ00);
	}
	
	
	//Set UBRRH to the highest 4 bits
	UBRR0H = (UBRR0 >> 8);
	//Set UBRRL to the rest
	UBRR0L =  UBRR0;
	//Enables TX and RX
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	
}

void printOut(char data)
{
	while(!((UCSR0A) & (1<<UDRE0)))
	{
		;
	}
	UDR0 = data;
}

void prints(char* data)
{
	while(*data)
	{
		printOut(*data++);
	}
}
