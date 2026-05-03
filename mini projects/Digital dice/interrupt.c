#include<lpc21xx.h>
#include "header.h"

unsigned int dice1;
unsigned int dice2;
extern int flag1;
extern int flag2;

void eint0_isr(void) __irq
{	
	flag1=1;
	dice1=(T1TC%6)+'1';
	EXTINT=1<<0;
	VICVectAddr=0;
}

void eint1_isr(void) __irq
{	
	flag2=1;
	dice2=(T1TC%6)+'1';
	EXTINT=1<<1;
	VICVectAddr=0;	
}

void int_config(void){
	PINSEL0|=0x20000000; //p0.14 as eint 1 (10 in pinsel 0 [29:28])
	PINSEL1|=0x01; 		//p0.16 as eint 0 (01 in pinsel 1 [1:0])
	
	VICIntSelect=0;     //all 32 interrupts as irqs
	
	//assigning eint0 to slot 0
	VICVectCntl0=(1<<5)|14;
	VICVectAddr0=(unsigned long)eint0_isr;
	
	//assigning eint1 to slot 1
	VICVectCntl1=(1<<5)|15;
	VICVectAddr1=(unsigned long)eint1_isr;	
	
	//configuring triggering of interrupts
	EXTMODE=0x0;
	EXTPOLAR=0x00;
	
	//enabling the interrupts
	VICIntEnable=(1<<14)|(1<<15);	
}
