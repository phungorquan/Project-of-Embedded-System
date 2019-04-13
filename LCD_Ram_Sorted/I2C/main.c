#include "stm32f10x.h"
#include "lcd.h"
#include "keyboard.h"
#include "setup.h"
#include "ram.h"


uint8_t _readdata;
int i = 0;
int count = 0;
char Stringkey[100] = {'\0'};


bool Mode_String = true;
bool Mode_Char = true;


void EmptyString(void) {
  int z;
  for (z = 0; z < 50; z++)
    Stringkey[z] = '\0';
}



int main() {
  uint8_t dataread[10];
	
	// Init GPIO
	GPIO_Setup();
	Delay(2000);
	
	// Init RAM
	SPI1_RAM_Init();
	Delay(10000);
	
	// Init UART2
	NVIC_Configuration();
  EnableUart_2();
	Delay(2000);
	
	// Init LCD
	I2C_LCD_Init();
	Delay(2000);
  LCD_Init();
	Delay(2000);
	Inform_Select_Mode();
	
 

  while (1) {

    //			if(_readdata == F1)
    //				{
    //					_readdata = 0;
    //					//if(Mode_Char == true)	// this function will run once
    //					//{
    //					Delete_LCD();
    //					lcd_goto_XY(1,0);
    //					lcd_send_string (" --- Select --- ");
    //					lcd_goto_XY(0,0);
    //					lcd_send_string ("  Input String");
    //					delaychung(10000);
    //					Delete_LCD();
    //						
    //					_send(NewLine);			
    //					_SendString("This is mode input a String , I'm typing ... ");
    //					_send(NewLine);
    //					
    //					//}
    //					Mode_String = true;
    //					Mode_Char = false;
    //					
    //				}

    if (_readdata == F2) {

      //if(Mode_String == true)	// this function will run once
      //{
      //Inform_Select_Mode();
      Delay(10000);
      Delete_LCD();

      _SendByte(NewLine);
      _SendString("This is mode input Character : ");
      _SendByte(NewLine);

      //}
      Mode_String = false;
      Mode_Char = true;
      _readdata = '0';

    } else if (_readdata == ESC) {
      Mode_Char = false;
      EmptyString();
      Inform_Select_Mode();

      i = 0;
      count = 0;
      _readdata = '0';
    } else if (_readdata == ENTER && Mode_Char == true) {

      _SendString(Stringkey);
      Inform_Select_Mode();
			EmptyString();
      Mode_Char = false;

      i = 0;
      count = 0;
      _readdata = '0';
    }
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0)
			{
				uint8_t datastr1[] = "Men";
				uint8_t datastr2[] = "Toss";
		
				//uint8_t data = 0;                                   // initialize the data
					//int i;
					//uint32_t address = 0;
				while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0){};
				_SendByte('Q');
					
				SetMode(Sequential);                           // set to send/receive single byte of data
				
				//for (i = 0; i <= 5; i++) {                  // Let's write 5 individual bytes to memory
      //address = i;                              // use the loop counter as the address
      WriteArray(0,datastr1,3);                // now write the data to that address
      Delay(200);
			WriteArray(3,datastr2,4);                // now write the data to that address
      Delay(200);
      //data += 2;                                // increment the data by 2
    //}
					
				//delay(1000);
			}
			else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0)
			{int i;
			
					//uint8_t value;
					
					//uint32_t address = 0;
				while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0){};
					_SendByte('X');
					SetMode(Sequential);                        // set to send/receive single byte of data
					
				//for (i = 0; i <= 5; i++) {                  // start at memory location 0 and end at 5
				//address = i;                              // use the loop counter as the memory address
				ReadArray(0,dataread,10);                // reads a byte of data at that memory location
				for(i = 0;i<10;i++)
					{
						_SendByte(dataread[i]);
						Delay(20);
					}
					//delay(200);
				//_send(value);                    // Let's see what we got
			//}
			}
  }
	
}
	


void USART2_IRQHandler(void) {
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //enter interrupt when STM32 receice data.
  {
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    _readdata = USART_ReceiveData(USART2);

    //				if(Mode_String == true && _readdata != ENTER)
    //				{
    //					Stringkey[i] = Letter[_readdata];
    //					i++;
    //				}
    //				else if (Mode_String == true && _readdata == ENTER)
    //				 {
    //					 //_SendString(Stringkey);	
    //					Delete_LCD();
    //					lcd_goto_XY(1,0);
    //					 lcd_send_string(Stringkey);
    //						Stringkey[0] = '\0'; 	// clear char array
    //						i = 0;			 
    //				 }

    if (Mode_Char == true && _readdata != '0') {
      if (count == 16) {

        Delete_LCD();
        LCD_SetCurSor_XY(1, 0);
        count = 0;

      }
      if (_readdata != ENTER && _readdata != '0') {
        if (_readdata != ESC) {
          char tmp = Letter[_readdata];
          LCD_Print(tmp);
          Stringkey[i] = tmp;
          i++;
          count++;
        }
      }

    }

  }
}

