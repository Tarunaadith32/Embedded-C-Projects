#include<lpc21xx.h>
void msdelay(unsigned int n){
	T0PR=15000-1;
	T0TCR=0x01;
	while(T0TC<n);
	T0TCR=0x03;
	T0TCR=0x00;
}

