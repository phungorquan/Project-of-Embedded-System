#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "setup.h"
#include "23LC1024.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
		
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}


	//Variables to hold the readback data
	uint8_t B = 0;
	uint8_t C = 0;
	uint16_t W = 0;
	uint32_t D = 0, Cnt = 0;
	float F = 0.0f;

	char buffer[] = "TUAN VO DEP TRAI CUTE";
	char buffer2[100];
int main(void)
{
	Init_USART();
	Init_GPIO();
	S23LC_Init();

//	//Write initial data to the SPI RAM or each size
//	S23_WByte(0, 'A');
//	S23_WByte(50, 'C');
//	S23_WWord(1, 1234);
//	S23_WDWord(3, 12345678);
//	S23_WFloat(7, 1.2345);
//	
//			//Read back all the initially stored data
//		B = S23_RByte(0);
//		C = S23_RByte(50);
//		W = S23_RWord(1);
//		D = S23_RDWord(3);
//		F = S23_RFloat(7);
//	
//	printf("%c",(char)B);
//	printf("%c",(char)C);
	
	WriteBuffer(buffer);
	for(uint8_t i = 0; i< strlen(buffer); i++)
		{
			buffer2[i] = S23_RByte(i);
			printf("%c", (char)buffer2[i]);
		}
		printf("\n");
	
	while(1)
	{		
			//Set test pin high!
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);

		//Execute speed test
		S23_SpeedTest();
		Delay_ms(100);

		//Reset test pin
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		
		//Using a logic analyzer, one can look at the duration that this pin is
		//high and use this to calculate the bit rate, using S_SIZE as the amount
		//of data transferred

	}
}



