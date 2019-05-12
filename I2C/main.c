#include "stm32f10x.h"
#include "lcd.h"
#include "keyboard.h"
#include "setup.h"
#include "ram.h"
#include "romi2c.h"

#define _ID		0x00
#define _Pass	0x01

uint8_t _readdata;
int indexofString = 0;
int count = 0;
int Str_Len_OfString = 0;
char Stringkey[100] = {'\0'};


bool Mode_String = true;
bool Mode_Char = true;


#define  EEP_Firstpage      0x00
uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];


void EmptyString(void) {
  int z;
  for (z = 0; z < Str_Len_OfString; z++)
    Stringkey[z] = '\0';
}

bool Check(uint8_t Mode)
{
	 
		char Buffer[4];
		int k = 0;
		uint8_t _Userinput;
		char tmp;
		
	_SendString("CHECK\r\n");
	while(_Userinput != ENTER)
	{
			if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //enter interrupt when STM32 receice data.
				{
					USART_ClearITPendingBit(USART2, USART_IT_RXNE);
					_Userinput = USART_ReceiveData(USART2);
					tmp = Letter[_Userinput];
		
					if(k < 5)
							{
								if(_Userinput != ENTER)
									{
										Buffer[k] = tmp;
										k++;
										LCD_Print(tmp);
									}
							}
				}
}

	if(Mode == _ID)
	{
		if(strcmp("ABCDE",Buffer) == 0) // = 0 is as same as each other
			return true;
	}
	
	else if(Mode == _Pass)
	{
		if(strcmp ("12345",Buffer) == 0) // = 0 is as same as each other
			return true;
	}
	
	return false;
	
}

void I2C_Test(void)
{
	u16 i;

    
	for ( i=0; i<=10; i++ ) //????
  {   
   I2c_Buf_Write[i] = 46+i;
  
   }

  //?I2c_Buf_Write??????????EERPOM? 
	I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 10);	 
  
  //?EEPROM?????????I2c_Buf_Read? 
	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 

  //?I2c_Buf_Read??????????
	for (i=0; i<256; i++)
	{	
//		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
//		{
//			_send(I2c_Buf_Read[i]);
//			return;
//		}
    _SendByte(I2c_Buf_Read[i]);
    if(i%16 == 15)    
        _SendString("\n\r");
   
	}
}

int main() {
	EnableUart_2();
	Delay(2000);
	 _SendString("START\r\n");
	Delay(2000);
	
	I2C_EE_Init();
	Delay(2000);
	I2C_Test();
	Delay(2000);
	_SendString("ROM OK\r\n");
	
	// Init GPIO
	GPIO_Setup();
	Delay(2000);
	
	_SendString("GPIO OK\r\n");
	
	// Init RAM
	SPI1_RAM_Init();
	Delay(10000);
	_SendString("RAM OK\r\n");
	
	// Init UART2
	//NVIC_Configuration();
  //EnableUart_2();
	//Delay(2000);
	
	// Init LCD
	I2C_LCD_Init();
	Delay(2000);
  LCD_Init();
	Delay(2000);
	_SendString("LCD OK\r\n");
	//Inform_Select_Mode();
	
			
		do{
			LCD_Clear();
			Delay(30);
			LCD_SetCurSor_XY(1, 0);
			Delay(30);
			LCD_Print_String("ID:");
		}while(Check(_ID) == false);

			
				do{
				LCD_SetCurSor_XY(0, 0);
				Delay(30);
				LCD_Print_String("Pass:");
				LCD_SetCurSor_XY(0, 5);
				Delay(30);
				LCD_Print_String("     ");
				LCD_SetCurSor_XY(0, 5);
				}while(Check(_Pass) == false);
				
				Inform_Select_Mode();
				NVIC_Configuration();
				
	
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

    if (_readdata == F2) 
			{

      //if(Mode_String == true)	// this function will run once
      //{
      //Inform_Select_Mode();
      Delay(10000);
      LCD_Clear();

      _SendByte(NewLine);
      _SendString("This is mode input Character : ");
      _SendByte(NewLine);

      //}
      Mode_String = false;
      Mode_Char = true;
      indexofString = 0;
      count = 0;
      _readdata = '0';

    } 
		else if (_readdata == ESC) 
			{
      Mode_Char = false;
      EmptyString();
      Inform_Select_Mode();

      indexofString = 0;
      count = 0;
      _readdata = '0';
    } 
		else if (_readdata == ENTER && Mode_Char == true) 
			{

       _SendString(Stringkey);
				
				
				SetMode(Sequential);                           // set to send/receive single byte of data
				Str_Len_OfString = indexofString;
				//WriteArray(0,Stringkey,Str_Len_OfString);                // now write the data to that address
				Delay(100);

			Inform_Select_Mode();
			EmptyString();
      Mode_Char = false;

      indexofString = 0;
      count = 0;
      _readdata = '0';
    }
		
			
		else if(_readdata == F3)
			{
				int k;
				char dataread[100];
					_SendByte('X');
					SetMode(Sequential);                        // set to send/receive single byte of data

				ReadArray(0,dataread,Str_Len_OfString);                // reads a byte of data at that memory location
				// Send Whole of String will be error with cache
				for(k = 0;k<Str_Len_OfString;k++)
					{
						_SendByte(dataread[k]);
						Delay(20);
					}
					
			//Mode_Char = false;

      indexofString = 0;
      count = 0;
      _readdata = '0';
			}
			
			else if(_readdata == F4)
			{
				
				char dataread1[16];
				char dataread2[16];
				uint8_t a = Str_Len_OfString%16;
				uint8_t b = Str_Len_OfString/16;
				
				uint8_t k =0;
				uint8_t z =0;
				
				uint8_t Change_Check = 1;
				
				
				if( a != 0)
					b++;
				SetMode(Sequential);
				
				for(k = 0 ;k<b;k++)
				{
					if (k%2==0)
					{
						LCD_Clear();
						Delay(5000);
					}
					
					if((Str_Len_OfString - 16*k) >= 16 )
					{
							Delay(50);
							ReadArray(k*16,dataread1,16);                // reads a byte of data at that memory location
							Delay(50);
							LCD_SetCurSor_XY(Change_Check,0);
							Delay(50);
							// Send Whole of String here will be Because it doesn't matter with full 16col
							LCD_Print_String(dataread1);
							Delay(15000);
							Change_Check ^= 1;
					}
					else if((Str_Len_OfString - 16*k) < 16 )
					{
							Delay(50);
							ReadArray(k*16,dataread2,Str_Len_OfString - 16*k);                // reads a byte of data at that memory location
							Delay(50);
							LCD_SetCurSor_XY(Change_Check,0);
							Delay(50);
							
							// Send Whole of String will be error with cache
							for(z = 0;z<Str_Len_OfString - 16*k;z++)
							{
								LCD_Print(dataread2[z]);
								Delay(20);
							}
							
							for(z = 0;z<16- (Str_Len_OfString - 16*k);z++)
							{
								LCD_Print(' ');
								Delay(20);
							}
							
							//LCD_Print_String(dataread2);
							Delay(15000);				
					}

					
				}
				

      indexofString = 0;
      count = 0;
      _readdata = '0';
			
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

        LCD_Clear();
        LCD_SetCurSor_XY(1, 0);
        count = 0;

      }
      if (_readdata != ENTER && _readdata != '0') {
        if (_readdata != ESC) {
          char tmp = Letter[_readdata];
					SetMode(ByteMode);
          LCD_Print(tmp);
          Stringkey[indexofString] = tmp;
					WriteByte(indexofString,tmp);
          indexofString++;
          count++;
        }
      }

    }

  }
}

