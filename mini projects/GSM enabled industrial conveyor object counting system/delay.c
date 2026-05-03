#include <lpc21xx.h>
#include "delay.h"

void msdelay(unsigned int i){
	T0PR=15000-1;
	T0TCR=0x01;
	while(T0TC<i);
	T0TCR=0x03;
	T0TCR=0x00;
}
