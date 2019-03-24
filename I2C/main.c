#include "stm32f10x.h"
#include "lcd.h"
#include "keyboard.h"

uint8_t _readdata;
int i = 0;
char Stringkey[10];

bool Mode_String = true;
bool Mode_Char = true;


char Letter[] = {
	
	'/','/','/','/','/'/*F3*/,' '/*F1*/,' '/*F2*/,'/','/','/','/','/','/','/','`','/','/','/','/' /*LSHIFT*/,'/', '/'/*LCRTL*/
	,'Q','1','/','/','/','Z','S','A','W','2','/','/','C','X','D','E','4','3','/','/'
	,' ' /*SPACE*/,'V','F','T','R','5','/','/','N','B','H','G','Y','6','/','/','/','M','J','U'
	,'7','8','/','/','/','K','I','O','0','9','/','/','/','/','L','/','P','-','/','/'
	,'/','/','/','/','='
};


	
int main(void)
{
	i2c_init();
	lcd_init();
	lcd_goto_XY(1,0);
	lcd_send_string ("F1-inputstring");
	lcd_goto_XY(0,0);
	lcd_send_string ("F2-inputchar");
	NVIC_Configuration();
	EnableUart_2();
	//Setdefaultled();   
			
    while (1)
    {
			
			if(_readdata == F1)
				{
					if(Mode_Char == true)
					{
					_send(0x0A);
					_SendString("This is mode input a String , I'm typing ... ");
					_send(0x0A);
					
					}
					Mode_String = true;
					Mode_Char = false;
					
				}
				
			else if (_readdata == F2)
			{
				if(Mode_String == true)
				{
				_send(0x0A);
				_SendString("This is mode input Character : ");
				_send(0x0A);
					
				}
				Mode_String = false;
				Mode_Char = true;
				
			}
		}
}


void USART2_IRQHandler(void)
{
	 if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//enter interrupt when STM32 receice data.
      {
         USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        _readdata = USART_ReceiveData(USART2); 
				
				if(Mode_String == true && _readdata != 0x5A)
				{
					Stringkey[i] = Letter[_readdata];
					i++;
				}
				else if (Mode_String == true && _readdata == 0x5A)
				 {
					 //_SendString(Stringkey);	
					Delete_LCD();
					lcd_goto_XY(1,0);
					 lcd_send_string(Stringkey);
						Stringkey[0] = '\0'; 	// clear char array
						i = 0;			 
				 }
				
				else if (Mode_Char == true)
				{
					//_send(Letter[_readdata]);
					lcd_Data_Write(Letter[_readdata]);
				}
				
			}
}

