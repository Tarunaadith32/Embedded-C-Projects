#include <lpc21xx.h>
#include "delay.h"
#define pins 0xf<<7 //lcd pins p0.7- p0.10
#define rs 1<<11 
#define en 1<<12

void lcd_cmd(unsigned char data);
void lcd_data(unsigned char data);

void lcd_init(void){
	IODIR0|=pins|rs|en;
	lcd_cmd(0x02);
	lcd_cmd(0x28);
	lcd_cmd(0x0c);
	lcd_cmd(0x01);
	lcd_cmd(0x80);
}

void lcd_cmd(unsigned char data){
	IOCLR0=pins;
	IOSET0=((data>>4)&0xf)<<7;
	IOCLR0=rs;
	IOSET0=en;
	msdelay(2);
	IOCLR0=en;
	IOCLR0=pins;
	IOSET0=(data&0xf)<<7;
	IOCLR0=rs;
	IOSET0=en;
	msdelay(2);
	IOCLR0=en;	
}

void lcd_data(unsigned char data){
	IOCLR0=pins;
	IOSET0=((data>>4)&0xf)<<7;
	IOSET0=rs;
	IOSET0=en;
	msdelay(2);
	IOCLR0=en;
	IOCLR0=pins;
	IOSET0=(data&0xf)<<7;
	IOSET0=rs;
	IOSET0=en;
	msdelay(2);
	IOCLR0=en;	
}

void int_display(unsigned int num){
	unsigned int arr[10];
	unsigned int count=0;
	int i;
	if (num==0){
		lcd_data('0');
		return;
	}
	for(i=0;(num&&(i<10));i++){
		arr[i]=(num%10+'0');
		count++;
		num/=10;
	}
	while(count--){
		lcd_data(arr[count]);
	}
}

void float_display(float num){
	int n;
	int decimal;
	if(num<0) num=0.0f;	
	n=(int)num;
	
	int_display(n);
	lcd_data('.');
	decimal=(int)(((num-n)*10));
	int_display(decimal);
	}
	

void lcd_str(char* s){
	while(*s) lcd_data(*s++);
}

void fuel_display(unsigned int n){
	unsigned int val=(n*100)/4095;
	lcd_str("FUEL : ");
	int_display(val);
	lcd_data('%');
	if(val<20) lcd_str(" LOW!!");
}

void temp_display(unsigned int n){
	float val=((n/4095.0)*3.3)*100;
	lcd_str("TEMP : ");
	float_display(val);
	lcd_data('c');
}

void dist_display(unsigned int n){
	lcd_str("DIST : ");
	if(n!=1000){
	int_display(n);
	lcd_str("cm");
	}
	else lcd_data('-');
	
}
