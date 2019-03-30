//setup.c
#include "setup.h"

GPIO_InitTypeDef 			GPIO_InitStructure;	/* Private typedef */
USART_InitTypeDef			UART_InitStructure;
NVIC_InitTypeDef			NVIC_InitStructure;
SPI_InitTypeDef   		SPI_InitStructure;

void Delay_ms(unsigned int nCount)
{ 
	unsigned int i, j;
	for(i = 0; i < nCount; i++)
   {  
	  for(j = 0; j < 0x2AFF; j++){;} 
   }
} 


void Init_USART(void)
{

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);/* USART2 Periph clock enable */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
			/* Cau Tx mode AF_PP, Rx mode FLOATING  */
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
		/*Cau hinh USART*/
			UART_InitStructure.USART_BaudRate = 115200;
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

void Init_GPIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

void Init_SPI1_Master(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOA, ENABLE);
		
	//PA4 : NSS
	//PA5 : SCK
	//PA6 : MISO
	//PA7 : MOSI
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_7| GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		//SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI1, &SPI_InitStructure);
		SPI_Cmd(SPI1, ENABLE);

}
