#include <lpc21xx.h>
#include "gsm.h"
#include "uart_define.h"
#include "delay.h"

void send_msg(int count){
	uart_str("AT");
	uart_tx(0x0d);
	msdelay(500);
	uart_str("AT+CMGF=1");
    uart_tx(0x0d);
    msdelay(500);
    uart_str("AT+CMGS=");
	uart_tx('"');
	uart_str("7695904608");
	uart_tx('"');
    uart_tx(0x0d);
    msdelay(500);
	uart_str("Conveyer counter count : ");
	uart_int(count);
    uart_tx(0x1a);
    msdelay(500);
}
