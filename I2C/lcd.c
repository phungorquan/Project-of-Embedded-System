#include "lcd.h"


void I2C_LCD_Init(void)
{
		GPIO_InitTypeDef 					GPIO_InitStructure;
		I2C_InitTypeDef						I2C_InitStructure;   
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);							
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // Config SDA, SCL //PB10 - SCL, PB11 - SDA
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;						
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Config I2C
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0; // 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;       
    I2C_InitStructure.I2C_ClockSpeed = 100000;										
    
    // Enable I2C
		I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);

}

void LCD_Init (void)
{
	/* Set 4-bits interface */
	LCD_Control_Write(0x33);		 
	Delay(10);
	
	LCD_Control_Write(0x32);
	Delay(50);
	
	/* Start to set LCD function */
	LCD_Control_Write(0x28);
		Delay(50);	
	
	/* clear LCD */
	LCD_Control_Write(0x01);
		Delay(50);
	
	/* set entry mode */
	LCD_Control_Write(0x06);	
	Delay(50);
	
	/* set display to on */
	LCD_Control_Write(0x0C);	
	Delay(50);
	
	/* move cursor to home and set data address to 0 */
	LCD_Control_Write(0x02);	
	Delay(50);
}


void LCD_Print(char data)
{
	char data_u, data_l;
	uint8_t data_t[4],i=0;
	
	data_u = data&0xf0;
	data_l = (data<<4)&0xf0;
	data_t[0] = data_u|0x0d;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0d;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	
	for(i = 0;i<4;i++)
		LCD_Write_Byte(data_t[i]);
	

}

void LCD_Print_String (char *str)
{
	while (*str) 
		LCD_Print (*str++);
}

void LCD_Clear(void)
{
	LCD_Control_Write(0x01);
	Delay(50);
}

void LCD_SetCurSor_XY (int row, int col)
{
	uint8_t pos_Addr;
	
	if(row == 1) 
		pos_Addr = 0x80 + row - 1 + col;
	else
		pos_Addr = 0x80 | (0x40 + col);
	
	LCD_Control_Write(pos_Addr);
}

void LCD_Write_Byte(char data)
{
    /* Send START condition */
    I2C_GenerateSTART(I2C_Channel, ENABLE);
    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_MODE_SELECT));
    /* Send PCF8574A address for write */
    I2C_Send7bitAddress(I2C_Channel, LCD_I2C_Address, I2C_Direction_Transmitter);
	/* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    /* Send the byte to be written */
    I2C_SendData(I2C_Channel, data);
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    /* Send STOP condition */
    I2C_GenerateSTOP(I2C_Channel, ENABLE);		
}


void LCD_Control_Write(char data)
{
  char data_u, data_l;
	uint8_t data_t[4],i=0;
	
	data_u = data&0xf0;
	data_l = (data<<4)&0xf0;
	data_t[0] = data_u|0x04;  //en=1, rs=0
	data_t[1] = data_u;  			//en=0, rs=0
	data_t[2] = data_l|0x04;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	
	for(i = 0;i<4;i++)
		LCD_Write_Byte(data_t[i]);

}


void Inform_Select_Mode()
{
	LCD_Clear();
	Delay(100);
  LCD_SetCurSor_XY(1, 0);
	Delay(100);
  LCD_Print_String("F1:INPUT-F2:SHOW");
  LCD_SetCurSor_XY(0, 0);
	Delay(100);
  LCD_Print_String("F3:SET-F4:CHANGE");
}


