#ifndef lcd_h
#define lcd_h

#include <lpc21xx.h>
#include "delay.h"
#define pins (0xf<<2) //lcd pins p0.2- p0.5
#define rs (1<<8) 
#define en (1<<9)

unsigned char fan[8]={0x00,0x04,0x15,0x0E,0x15,0x04,0x00,0x00};

void lcd_cmd(unsigned char data);
void lcd_data(unsigned char data);
void lcd_str(char* data);



void lcd_init(void){
	int i;
	IODIR0|=pins|rs|en;
	lcd_cmd(0x02);
	lcd_cmd(0x28);
	lcd_cmd(0x0c);
	lcd_cmd(0x01);
	lcd_cmd(0x80);
	lcd_str("*****DRIVESENSE*****");
	lcd_cmd(0xc0);
	lcd_str("INTELLIGENT VEHICLE ");
	lcd_cmd(0x94);
	lcd_str("PARAMETER MONITORING");
	lcd_cmd(0xd4);
	lcd_str("       SYSTEM       ");
	msdelay(3000);
	lcd_cmd(0x01);
	lcd_cmd(0x40);
	for(i=0;i<8;i++) lcd_data(fan[i]);
}

void lcd_cmd(unsigned char data){
	IOCLR0=pins;
	IOSET0=((data>>4)&0xf)<<2;
	IOCLR0=rs;
	IOSET0=en;
	msdelay(2);
	IOCLR0=en;
	IOCLR0=pins;
	IOSET0=(data&0xf)<<2;
	IOCLR0=rs;
	IOSET0=en;
	msdelay(2);
	IOCLR0=en;	
}

void lcd_data(unsigned char data){
	IOCLR0=pins;
	IOSET0=((data>>4)&0xf)<<2;
	IOSET0=rs;
	IOSET0=en;
	msdelay(2);
	IOCLR0=en;
	IOCLR0=pins;
	IOSET0=(data&0xf)<<2;
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

#endif
