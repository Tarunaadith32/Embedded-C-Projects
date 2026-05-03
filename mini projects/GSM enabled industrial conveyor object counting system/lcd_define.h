#ifndef lcd
#define lcd

void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_str(char*);
void lcd_int(int);
void display(int);

#define lcd_pins 0xff<<2
#define rs 1<<10
#define en 1<<11

#endif
