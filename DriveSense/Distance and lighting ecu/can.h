#ifndef can_h
#define can_h

#include <lpc21xx.h>
#include "delay.h"

typedef struct can_node{
	unsigned int id;
	unsigned int rtr;
	unsigned int dlc;
	unsigned int byte_a;
	unsigned int byte_b;
	
}can2;

void can_init(void){
	PINSEL1|=0x00014000; //P0.23 and P0.24 CAN Td and Rd
	VPBDIV=0x1; //PCLK 60mhz
	C2MOD=0x1; // Reset mode
	AFMR=0x2;// accept all messages
	C2BTR=0x001c001d; //125kbps (pclk/(speed*16))
	C2MOD=0x00; // normal mode
}

void can2_tx(can2 msg){
	C2TID1=msg.id;
	C2TFI1=(msg.dlc<<16);
	if(msg.rtr==0){
		C2TFI1&=~(1<<30);
		C2TDA1=msg.byte_a;
		C2TDB1=msg.byte_b;
	}
	else{
		C2TFI1|=(1<<30);
	}
	C2CMR=(1<<0)|(1<<5); //start tx and select tx channel 1
	while(((C2GSR>>3)&1)==0); //wait to complete tx
}

void can2_rx(can2 *msg){
	while((C2GSR&1)==0); //wait for receive 
	msg->id=C2RID;
	msg->dlc=(C2RFS>>16)&0xf;
	msg->rtr=(C2RFS>>30)&0x1;
	if(msg->rtr==0){
		msg->byte_a=C2RDA;
		msg->byte_b=C2RDB;
	}
	C2CMR=1<<2; // free receive buffer
}

#endif
