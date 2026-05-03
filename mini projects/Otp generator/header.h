#ifndef header_h
#define header_h

#include <lpc21xx.h>

void uart_isr(void) __irq;

char* otpgen(void);


void uart_config(void);
void uart_tx(unsigned char);
unsigned char uart_rx(void);
void uart_intconfig(void);
void uart_str(char*);
void otp_clr(void);


void msdelay(unsigned int);

#endif
