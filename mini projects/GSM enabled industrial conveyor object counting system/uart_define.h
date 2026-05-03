#ifndef uart
#define uart

void uart_config(void);
void uart_tx(unsigned char);
unsigned char uart_rx(void);
void uart_str(char*);
void uart_int(int);

#endif
