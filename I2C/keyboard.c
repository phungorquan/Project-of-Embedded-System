#include "keyboard.h"

GPIO_InitTypeDef GPIO_InitStruct; 
USART_InitTypeDef USART_InitStructure;


void _send(uint8_t data)
{
 USART_SendData(USART2, data);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void _SendString(char str[])
{
   int i =0;
  
  while (str[i] != 0x00)
  {
    _send(str[i]);
    i++;
  }
}


//void SetDefaultLed()
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;//|GPIO_Pin_1;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
//	
//	GPIO_Init(GPIOC, &GPIO_InitStruct); 
//	GPIO_ResetBits(GPIOC, GPIO_Pin_13);//|GPIO_Pin_1); 
//}



void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


void EnableUart_2()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

		// Enable TX
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    //Enable RX
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
        
	// CONFIG USART
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	// Enable UART
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
}
