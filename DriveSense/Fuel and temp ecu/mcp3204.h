#ifndef mcp3204_h
#define mcp3204_h

#include <LPC21XX.H>
#include "spi.h"


unsigned int mcp3204_read(unsigned char ch){
	unsigned char hbyte,lbyte;
	unsigned int adcval=0;
	IOCLR0=1<<7;
	spi_tx(0x06);
	hbyte=spi_tx(ch<<6);
	lbyte=spi_tx(0x00);
	IOSET0=1<<7;
	adcval=(((hbyte&0xf)<<8)|lbyte);
//	if(adcval>4095) adcval=4095;
	return adcval;
	
}

#endif
