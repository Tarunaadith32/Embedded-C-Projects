#include <lpc21xx.h>
#include "delay.h"
#include "lcd_define.h"
#include "uart_define.h"
#include "int_config.h"
#include "gsm.h"


#define ir 12
#define motor_in1 13
#define motor_in2 14

int main(){
	int count=0;
	unsigned int t0;
	int lcdcnt;
	int prev=1,curr=0;
	
	//Timer 1 for counting 
	T1PR=15000-1;
	T1TCR=0x01;
	
	//configuring motor in forward direction
	IODIR0|=(1<<13)|(1<<14);
	IOSET0=1<<13;
	IOCLR0=1<<14;
	
	lcd_init();
	int_config();
	uart_config();
	
	lcd_cmd(0x80);
	lcd_str(" OBJECT COUNTER ");
	
	while(1){
		curr=IOPIN0>>ir&1;
		if((prev==1)&&(curr==0))count++;
		prev=curr;
		if(flag==1){
			t0=T1TC;
			display(count);
			send_msg(count);	
			lcdcnt=1;
			flag=0;
		}
		if(T1TC-t0>=3000&&lcdcnt==1){
			lcd_cmd(0x80);
			lcd_str(" OBJECT COUNTER ");
			lcdcnt=0;
		}		
	}
}
