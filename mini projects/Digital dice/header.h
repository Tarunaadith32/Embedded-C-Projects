#ifndef header_h
#define header_h

void int_config(void);// function to configure eint0 and eint1
void timer_config(void); //function to configure timer1 for dice counting

void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_str(char*);
void cgram_write(void);

void timer_config(void);

void msdelay(unsigned int n);

#endif
