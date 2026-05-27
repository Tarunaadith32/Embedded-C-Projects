#include <lpc21xx.h>
#include "delay.h"
#include "display.h"
#include "can.h"
#include "lcd.h"

int main(){
	unsigned int data;
	can2 msg;
	IODIR0=1<<14; //p0.14 fuel led
	can_init();
	lcd_init();
	while(1){	
		can2_rx(&msg);
		if(msg.id==0x100){
			lcd_cmd(0x80);
			lcd_str("                ");
			lcd_cmd(0x80);
			fuel_display(msg.byte_a);
		}
		else if(msg.id==0x150){
			lcd_cmd(0xc0);
			lcd_str("                ");
			lcd_cmd(0xc0);
			data=(msg.byte_a|(msg.byte_b<<8));			
			temp_display(data);
		}
		else if(msg.id==0x200){
			lcd_cmd(0x94);
			lcd_str("                ");
			lcd_cmd(0x94);
			data=(msg.byte_a|(msg.byte_b<<8));			
			dist_display(data);
		}
		else if(msg.id==0x250){
			lcd_cmd(0xd4);
			lcd_str("                ");
			lcd_cmd(0xd4);
			data=(msg.byte_a|(msg.byte_b<<8));			
			light_display(data);
		}
		else continue;
	}
}
