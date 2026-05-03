#include<lpc21xx.h>
#include "header.h"

int flag1;
int flag2;
extern unsigned int dice1;
extern unsigned int dice2;
	
int main(){
	lcd_init();
	lcd_cmd(0x80);
	lcd_str("Player 1 : -");
	lcd_cmd(0xc0);
	lcd_str("Player 2 : -");
	int_config();
	timer_config();
	
	while(1){
		if(flag1==1){
			lcd_cmd(0x8b);
			lcd_data(dice1);
		//	msdelay(100);	
			flag1=0;
		}
		else if(flag2==1){
			lcd_cmd(0xcb);
			lcd_data(dice2);
		//	msdelay(100);
			flag2=0;
		}
	}
}
