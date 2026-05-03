#include <lpc21xx.h>
#include "int_config.h"

unsigned int time0=0;
unsigned int flag;

void eint0_isr(void) __irq{
	unsigned int time1 = T1TC;
	if(time1-time0>800){
		flag=1;
		time0=time1;
	}
	EXTINT=0x01;
	VICVectAddr=0;
}
 
 void int_config(void){
	 PINSEL1|=0x1;
	 
	 VICIntSelect=0;
	 VICVectCntl0=(1<<5)|14;
	 VICVectAddr0=(unsigned long)eint0_isr;
	 
	 EXTMODE=0x01;
	 EXTPOLAR=0x00;
	 
	 VICIntEnable=(1<<14);
	 
 }
 