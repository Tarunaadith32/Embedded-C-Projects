#include <lpc21xx.h>
#include "lcd_define.h"
#include "delay.h"


void lcd_init(void){
	IODIR0|= lcd_pins|rs|en;
	lcd_cmd(0x01);
	lcd_cmd(0x38);
	lcd_cmd(0x06);
	lcd_cmd(0x0c);
}

void lcd_cmd(unsigned char cmd){
	IOCLR0=lcd_pins;
	IOSET0=cmd<<2;
	IOCLR0=rs;
	IOSET0=en;
	msdelay(2);
	IOCLR0=en;
}

void lcd_data(unsigned char data){
	IOCLR0=lcd_pins;
	IOSET0=data<<2;
	IOSET0=rs;
	IOSET0=en;
	msdelay(2);
	IOCLR0=en;
}

void lcd_str(char* s){
	while(*s)
		lcd_data(*s++);
}
	
void lcd_int(int n){
	unsigned char a[5];
	int i=0;
	if(n==0) lcd_data('0');
	while(n){
		a[i++]=n%10+48;
		n/=10;
}
	for(--i;i>=0;i--){
		lcd_data(a[i]);
	}
}

void display(int count){
			lcd_cmd(0x80);
			lcd_str("Count : ");
			lcd_int(count);
			lcd_str("        ");
}
