#include<lpc21xx.h>
#include "header.h"

void timer_config(void){
	T1PR=15000-1;
	T1TCR=0x01;
}
