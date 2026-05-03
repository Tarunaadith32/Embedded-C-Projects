#include <lpc21xx.h>
#include "header.h"

unsigned int n;


void uart_isr(void) __irq{
	int temp,i;
	char c;
	char* otp;
	temp=U0IIR;
	if(!(temp&1))
		if(((temp>>1)&7)==2){
			c=U0RBR;
			if(c=='1'){
				otp = otpgen();
				otp_clr();
				uart_str("otp: ");
				for(i=0;i<6;i++){
					uart_tx(otp[i]);
				}
				uart_tx(0x0d);
			}
		}		
	VICVectAddr=0;	
}
		

int main(void){
	
	n=0;
	T1PR=15;
	T1TCR=0x01;
	uart_config();
	uart_intconfig();
	while(1);
}
