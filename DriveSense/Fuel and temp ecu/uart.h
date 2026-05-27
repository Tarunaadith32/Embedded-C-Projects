#include <lpc21xx.h>

//--------------------------------------------------
// UART0 INIT
//--------------------------------------------------
void uart0_init(void)
{
    PINSEL0 |= 0x00000005;   // P0.0 = TXD0 , P0.1 = RXD0

    U0LCR = 0x83;            // 8-bit, 1 stop, enable DLAB
    U0DLL = 0x86;              // 9600 baud for 15MHz PCLK
    U0DLM = 0x01;
    U0LCR = 0x03;            // disable DLAB
}

//--------------------------------------------------
// UART TX CHAR
//--------------------------------------------------
void uart0_tx_char(unsigned char ch)
{
    while(!(U0LSR & (1 << 5))); // wait till THR empty
    U0THR = ch;
}

//--------------------------------------------------
// UART TX STRING
//--------------------------------------------------
void uart0_tx_string(char *ptr)
{
    while(*ptr)
    {
        uart0_tx_char(*ptr++);
    }
}

//--------------------------------------------------
// UART TX NUMBER
//--------------------------------------------------
void uart0_tx_num(unsigned int num)
{
    char arr[10];
    int i = 0;

    if(num == 0)
    {
        uart0_tx_char('0');
        return;
    }

    while(num > 0)
    {
        arr[i++] = (num % 10) + '0';
        num /= 10;
    }

    while(i > 0)
    {
        uart0_tx_char(arr[--i]);
    }
}

//--------------------------------------------------
// UART HEX PRINT
//--------------------------------------------------
void uart0_tx_hex(unsigned int num)
{
    int i;
    unsigned char temp;

    uart0_tx_string("0x");

    for(i = 12; i >= 0; i -= 4)
    {
        temp = (num >> i) & 0x0F;

        if(temp < 10)
            uart0_tx_char(temp + '0');
        else
            uart0_tx_char(temp + 55);
    }
}
