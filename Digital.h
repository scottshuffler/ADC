/*
 * Digital.h
 *
 * Created: 2/12/2015 9:45:07 AM
 *  Author: shuffleres
 */ 


#ifndef DIGITAL_H_
#define DIGITAL_H_

#define HIGH 0x1
#define LOW 0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

struct PinMap
{
	volatile unsigned char* ddr;
	char bitNum;
	char bitShift;
};

//Array of strucs of all the ports with their pin#, bit number, and port
struct PinMap map[] = {
	{ &DDRE, 0, 0},
	{ &DDRE, 1, 1},
	{ &DDRE, 2, 4},
	{ &DDRE, 3, 5},
	{ &DDRG, 4, 5},
	{ &DDRE, 5, 3},
	{ &DDRH, 6, 3},
	{ &DDRH, 7, 4},
	{ &DDRH, 8, 5},
	{ &DDRH, 9, 6},
	{ &DDRB, 10, 4},
	{ &DDRB, 11, 5},
	{ &DDRB, 12, 6},
	{ &DDRB, 13, 7},
	{ &DDRJ, 14, 1},
	{ &DDRJ, 15, 0},
	{ &DDRH, 16, 1},
	{ &DDRH, 17, 0},
	{ &DDRD, 18, 3},
	{ &DDRD, 19, 2},
	{ &DDRD, 20, 1},
	{ &DDRD, 21, 0},
	{ &DDRA, 22, 0},
	{ &DDRA, 23, 1},
	{ &DDRA, 24, 2},
	{ &DDRA, 25, 3},
	{ &DDRA, 26, 4},
	{ &DDRA, 27, 5},
	{ &DDRA, 28, 6},
	{ &DDRA, 29, 7},
	{ &DDRC, 30, 7},
	{ &DDRC, 31, 6},
	{ &DDRC, 32, 5},
	{ &DDRC, 33, 4},
	{ &DDRC, 34, 3},
	{ &DDRC, 35, 2},
	{ &DDRC, 36, 1},
	{ &DDRC, 37, 0},
	{ &DDRD, 38, 7},
	{ &DDRG, 39, 2},
	{ &DDRG, 40, 1},
	{ &DDRG, 41, 0},
	{ &DDRL, 42, 7},
	{ &DDRL, 43, 6},
	{ &DDRL, 44, 5},
	{ &DDRL, 45, 4},
	{ &DDRL, 46, 3},
	{ &DDRL, 47, 2},
	{ &DDRL, 48, 1},
	{ &DDRL, 49, 0},
	{ &DDRB, 50, 3},
	{ &DDRB, 51, 2},
	{ &DDRB, 52, 1},
	{ &DDRB, 53, 0}
};

int state = 0;
int press = 0;

//Prototypes
void pinMode(int,char);
int digitalRead(int);
void digitalWrite(int,int);
void delay(int i);
#endif /* DIGITAL_H_ */