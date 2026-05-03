#include<lpc21xx.h>
#include "lcdfns.h"

//Print integer of variable sign.

/*int main(){
	int n=1234;
	LCD_INIT();
	LCD_CMD(0x80);
	LCD_STR("Number : ");
	LCD_INT(n);
}


//print float

int main(){
	float f=1.234;
	LCD_INIT();
	LCD_CMD(0x80);
	LCD_STR("Number : ");
	LCD_FLOAT(f);
}


//print binary

int main(){
	unsigned char n=3;
	LCD_INIT();
	LCD_CMD(0x80);
	LCD_STR("Binary:");
	LCD_BIN(n);
	}
	*/
	
	//display custom char

int main(){
		unsigned char a[]={0x04,0x0a,0x0a,0x0a,0x0A,0x15,0x0a,0x00,0x04,0x0a,0x0a,0x0a,0x0A,0x15,0x0a,0x00};
		LCD_INIT();
		CGRAM_DATA(a,16);
		LCD_CMD(0x80);
		LCD_DATA(0);	
		LCD_DATA(1);
}



	

