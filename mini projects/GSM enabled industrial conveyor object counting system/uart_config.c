#include <lpc21xx.h>
#include "uart_define.h"
void uart_config(void){
	PINSEL0|=0x5; //p0.0 tx, p0.1 rx
	U0LCR=0x83;
	U0DLL=97;
	U0DLM=0;
	U0LCR=0x03;
}

void uart_tx(unsigned char data){
	U0THR=data;
	while((U0LSR&(1<<5))==0);
}

unsigned char uart_rx(void){
	while((U0LSR&(1<<0))==0);
	return U0RBR;
}

void uart_str(char* s){
	while(*s)
		uart_tx(*s++);
}

void uart_int(int n){
	unsigned char a[5];
	int i=0;
	if(n==0){
		uart_tx('0');
		return;
	}
	while(n){
		a[i++]=n%10+48;
		n/=10;
}
	for(--i;i>=0;i--){
		uart_tx(a[i]);
	}
}
