#ifndef delay
#define delay

#include<lpc21xx.h>

void msdelay(unsigned int n){
	T0PR=60000-1;
	T0TCR=0x03;
	//T0TCR=0x02;
	T0TCR=0x01;
	while(T0TC<n);
	T0TCR=0x02;
	T0TCR=0x00;
}

void usdelay(unsigned int n){
	T0PR=60-1;
	T0TCR=0x03;
	//T0TCR=0x02;
	T0TCR=0x01;
	while(T0TC<n);
	T0TCR=0x02;
	T0TCR=0x00;
}
#endif
