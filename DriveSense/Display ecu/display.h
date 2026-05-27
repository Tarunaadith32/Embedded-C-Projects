#ifndef display_h
#define display_h

#include <lpc21xx.h>
#include "lcd.h"

void fuel_display(unsigned int n){
	unsigned int val=(n*100)/4095;
	lcd_str("FUEL : ");
	int_display(val);
	lcd_data('%');
	if(val<20){
		lcd_str(" LOW!!");
		IOCLR0=(1<<14);
	}
	else IOSET0=(1<<14);
}

void temp_display(unsigned int n){
	float val=((n/4095.0)*3.3)*100;
	lcd_str("TEMP : ");
	float_display(val);
	lcd_str("c ");
	if(val>30.0){ 
	lcd_data(0);
	lcd_str("ON");
	}
}

void dist_display(unsigned int n){
	lcd_str("DIST : ");
	if((n<=50)){
	int_display(n);
	lcd_str("cm");
	}
	else lcd_data('-');
	
}

void light_display(unsigned int n){
	lcd_str("LIGHT : ");
	if(n==0) lcd_str("OFF");
	else lcd_str("ON");
}

#endif
