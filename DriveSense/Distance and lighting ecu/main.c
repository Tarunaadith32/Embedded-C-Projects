#include <lpc21xx.h>
#include "delay.h"
#include "can.h"
#include "distance.h"

#define ldr (1<<5)

int main(){
	unsigned int data;
	can2 msg1,msg2;
	msg1.id=0x200;
	msg1.dlc=4;
	msg1.rtr=0;
	msg2.id=0x250;
	msg2.dlc=4;
	msg2.rtr=0;
	can_init();
	distance_init(); // distance sensor config
	IODIR0&=~ldr;    // ldr pin as input
	while(1){	
		data=get_dist(); //get distance data
		msg1.byte_a=data;
		can2_tx(msg1);
		
		if(IOPIN0&ldr)
			data=1; //read ldr data
		else data=0;
		msg2.byte_a=data;
		can2_tx(msg2);
		msdelay(1000);
	}
}
