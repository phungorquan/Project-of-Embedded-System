#include "stm32f10x.h"
#include "lcd.h"
#include "keyboard.h"


uint8_t _readdata;
int i = 0;
int count = 0;
char Stringkey[100] = {'\0'};


bool Mode_String = true;
bool Mode_Char = true;


char Letter[] = {
	
	'/','/','/','/','/'/*F3*/,' '/*F1*/,' '/*F2*/,'/','/','/','/','/','/','/','`','/','/','/','/' /*LSHIFT*/,'/', '/'/*LCRTL*/
	,'Q','1','/','/','/','Z','S','A','W','2','/','/','C','X','D','E','4','3','/','/'
	,' ' /*SPACE*/,'V','F','T','R','5','/','/','N','B','H','G','Y','6','/','/','/','M','J','U'
	,'7','8','/','/','/','K','I','O','0','9','/','/','/','/','L','/','P','-','/','/'
	,'/','/','/','/','='
};

void EmptyString(void) {
  int z;
  for (z = 0; z < 50; z++)
    Stringkey[z] = '\0';
}

void Inform_Select_Mode(void)
{
	Delete_LCD();
  lcd_goto_XY(1, 0);
  lcd_send_string("F1-inputstring");
  lcd_goto_XY(0, 0);
  lcd_send_string("F2-inputchar");
}

int main(void) {
  i2c_init();
  lcd_init();
	Inform_Select_Mode();
  NVIC_Configuration();
  EnableUart_2();
  //Setdefaultled();   

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
      Inform_Select_Mode();
      delaychung(10000);
      Delete_LCD();

      _send(NewLine);
      _SendString("This is mode input Character : ");
      _send(NewLine);

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
      //_send(Letter[_readdata]);
      if (count == 16) {

        Delete_LCD();
        lcd_goto_XY(1, 0);
        count = 0;

      }
      if (_readdata != ENTER && _readdata != '0') {
        if (_readdata != ESC) {
          char tmp = Letter[_readdata];
          lcd_Data_Write(tmp);
          Stringkey[i] = tmp;
          i++;
          count++;
        }
      }

    }

  }
}

