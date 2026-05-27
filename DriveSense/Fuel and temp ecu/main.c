#include <lpc21xx.h>
#include "delay.h"
#include "mcp3204.h"
#include "spi.h"
#include "can.h"
#include "uart.h"

int main(){
	unsigned int data;
	can2 msg1,msg2;
	msg1.id=0x100;
	msg1.dlc=4;
	msg1.rtr=0;
	msg2.id=0x150;
	msg2.dlc=4;
	msg2.rtr=0;
	spi_init();
	can_init();
	uart0_init();
	while(1){	
		data=mcp3204_read(0x00); //read fuel data
		//test	
		uart0_tx_string("Fuel ADC = ");
		uart0_tx_num(data);
		uart0_tx_string("\r\n");
       //endtest
		msg1.byte_a=data;
		can2_tx(msg1);
		data=mcp3204_read(0x01); //read lm35 temp data
		//test	
		uart0_tx_string("temp ADC = ");
		uart0_tx_num(data);
		uart0_tx_string("\r\n");
       //endtest
		msg2.byte_a=data;
		msg2.byte_b=(data>>8)&0xf;
		can2_tx(msg2);
		msdelay(1000);
	}
}
