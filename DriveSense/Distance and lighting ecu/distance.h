#ifndef dist_h
#define dist_h

#include <lpc21xx.h>
#include "delay.h"

#define trig (1<<3)
#define echo (1<<4)

void distance_init(){
	IODIR0|=trig;   // Trig pin as op
	IODIR0&=~echo;  // Echo pin as ip

}

unsigned int get_dist(void){
    unsigned int timeout;
    unsigned int dist;

    IOCLR0 = trig;
    usdelay(5);
    IOSET0 = trig;
    usdelay(15);
    IOCLR0 = trig;

    timeout = 0;
    while(!(IOPIN0 & echo)){
        timeout++;
        if(timeout > 50000) return 500;
    }

    T1PR  = 60 - 1;     // 1us per tick at PCLK=60MHz
    T1TCR = 0x03;        // reset T1TC to 0
    T1TCR = 0x01;        // start counting

    timeout = 0;
    while(IOPIN0 & echo){
        timeout++;
        if(timeout > 50000){
            T1TCR = 0x00;  // stop
            T1TCR = 0x02;  // reset
            return 1000;
        }
    }

    T1TCR = 0x00;        // stop
    dist = T1TC / 58;    // read THEN reset
    T1TCR = 0x02;        // reset T1TC
    T1TCR = 0x00;

    if(dist > 400) return 1000;
    return dist;
}

#endif
