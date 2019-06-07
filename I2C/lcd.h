#include "stm32f10x.h"
#include "setup.h"

#define LCD_I2C_Address	0x3F << 1
#define I2C_Channel	I2C2

// Init I2C
void I2C_LCD_Init(void);

// Init LCD
void LCD_Init (void);

// Print a byte to LCD
void LCD_Print(char );

// Print a string to LCD
void LCD_Print_String (char*);

// Clear LCD display
void LCD_Clear(void);

// Set Cursor , R C
void LCD_SetCurSor_XY (int, int ); 

// Support for the other LCD functions
void LCD_Write_Byte(char );
void LCD_Control_Write(char );

// Informe Select Mode
void Inform_Select_Mode(void);

