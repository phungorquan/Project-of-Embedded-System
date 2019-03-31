#include "stm32f10x.h"

#define I2C_Chanel      		I2C2
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef UART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
I2C_InitTypeDef I2C_InitStructure;

//------------------------------------------------

void GPIO_Config(void);
void UART_Config(void);
void Delay_ms(uint16_t time);
void Write_byte(char data);
char Read_byte(uint8_t high, uint8_t low);
void I2C_LCD_Configuration(void);
uint32_t getlastevent;
int main(void)
{
	GPIO_Config();
	UART_Config();
	I2C_LCD_Configuration();
	Write_byte('b');
	while(1)
	{
		//char t = Read_byte(0x00,0x60);
		USART_SendData(USART2,'a');
		Delay_ms(1000);
	}
}

void Delay_ms(uint16_t time)
{
	uint32_t time_n=time*12000;
	while(time_n!=0){time_n--;}
}
	
void GPIO_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

void UART_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*Cau hinh USART*/
	UART_InitStructure.USART_BaudRate = 19200;
	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_Parity = USART_Parity_No;
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &UART_InitStructure);
				
		/* Cau hinh vector ngat va muc uu tien */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
			
			/* xoa co ngat nhan cho lan dau su dung*/
	USART_ClearFlag(USART2, USART_IT_RXNE);
			
			/* Cau hinh cho phep ngat nhan*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

			/* Cho phep UART hoat dong */
	USART_Cmd(USART2, ENABLE);
}

void I2C_LCD_Configuration(void)
{
    // cap clock cho ngoai vi va I2C
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);							// su dung kenh I2C2 cua STM32
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // cau hinh chan SDA va SCL
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;						//PB10 - SCL, PB11 - SDA
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // cau hinh I2C
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0; // 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;       
    I2C_InitStructure.I2C_ClockSpeed = 100000;										
    I2C_Init(I2C2, &I2C_InitStructure);
    // cho phep bo I2C hoat dong
    I2C_Cmd(I2C2, ENABLE);
		I2C_AcknowledgeConfig(I2C_Chanel,ENABLE);
}

void Write_byte(char data)
{
    I2C_GenerateSTART(I2C_Chanel, ENABLE);
		while(!I2C_CheckEvent(I2C_Chanel,I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C_Chanel,0xA0, I2C_Direction_Transmitter);
	  while(I2C_CheckEvent(I2C_Chanel,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C_Chanel, 0x00);
		while(!I2C_CheckEvent(I2C_Chanel,I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	  //I2C_SendData(I2C_Chanel, 0x60);
	  //while(!I2C_CheckEvent(I2C_Chanel,I2C_EVENT_MASTER_BYTE_TRANSMITTING));
		//I2C_SendData(I2C_Chanel,data);
		//while(!I2C_CheckEvent(I2C_Chanel,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    /* Send STOP condition */
    I2C_GenerateSTOP(I2C_Chanel, ENABLE);		
}

char Read_byte(uint8_t high, uint8_t low)
{
	char a;
	I2C_GenerateSTART(I2C_Chanel, ENABLE);
  I2C_Send7bitAddress(I2C_Chanel,0xA0, I2C_Direction_Transmitter);
	Delay_ms(5);
  I2C_SendData(I2C_Chanel, high);
  Delay_ms(5);
	I2C_SendData(I2C_Chanel, low);
	Delay_ms(5);
	
	I2C_GenerateSTART(I2C_Chanel,ENABLE);
  I2C_Send7bitAddress(I2C_Chanel,0xA1, I2C_Direction_Receiver);
	Delay_ms(5);
	a = I2C_ReceiveData(I2C2);
	Delay_ms(5);
  I2C_GenerateSTOP(I2C_Chanel, ENABLE);		
	return a;
}




