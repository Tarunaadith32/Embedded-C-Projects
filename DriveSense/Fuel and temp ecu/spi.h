#ifndef spi_h
#define spi_h

#include <LPC21XX.H>
#include "delay.h"

void spi_init(void){
	PINSEL0|=((1<<8)|(1<<10)|(1<<12));
	S0SPCCR=150; //100kbps
	S0SPCR=(1<<5)|(1<<4)|(1<<3); // set mstr bit(5),cpha(4),cpol(3) high
								 // SPI master mode, msb first, mode 3 and 8 bit mode
	IODIR0|=1<<7; //P0.7 as cs
	IOSET0=1<<7;
}

unsigned char spi_tx(unsigned char data){
	unsigned char status;
	status=S0SPSR;
	S0SPDR=data;
	while(((S0SPSR>>7)&1)==0); //transfer complete flag
	return S0SPDR;
}

#endif
