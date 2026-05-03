#include <lpc21xx.h>
#include "header.h"

char otp[6];

char* otpgen(void){
	unsigned int i;
	for(i=0;i<6;i++){
		otp[i]=(T1TC%10)+'0';
	}
	return otp;
}
