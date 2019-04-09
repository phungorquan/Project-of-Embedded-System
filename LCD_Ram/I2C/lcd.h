#include "stm32f10x.h"
#define PCF8574A_Address	0x7E
#define I2C_Chanel	I2C2

void i2c_init(void);
void delaychung(int);

void lcd_Write_byte(char );
void lcd_Data_Write(char );
void lcd_Control_Write(char );
void lcd_init (void);

void lcd_send_string (char*);
void Delete_LCD(void);

void lcd_goto_XY (int, int ); //set proper location on screen
void Setdefaultled(void);