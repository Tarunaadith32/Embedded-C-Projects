#include <lpc21xx.h>
#include "header.h"

void uart_config(void){
	PINSEL0=(1<<0)|(1<<2);
	U0LCR=0x83;
	U0DLL=97;
	U0DLM=0;
	U0LCR=0x03;
}

void uart_tx(unsigned char data){
	U0THR=data;
	while(((U0LSR>>5)&1)==0);
}

unsigned char uart_rx(void){
	while(((U0LSR>>0)&1)==0);
	return U0RBR;
}

void uart_intconfig(void){
	VICIntSelect=0;
	VICVectCntl0=(1<<5)|6;
	VICVectAddr0=(unsigned long)uart_isr;
	U0IER=(1<<1)|(1<<0);
	VICIntEnable=1<<6;
}

void uart_str(char* s){
	while(*s)
		uart_tx(*s++);	
}

void otp_clr(void){
	int i;
	for(i=0;i<13;i++) uart_tx(0x08);
}
