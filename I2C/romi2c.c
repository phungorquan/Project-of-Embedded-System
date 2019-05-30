#include "romi2c.h"

#define I2C_Speed              400000
#define I2C1_OWN_ADDRESS7    0x0A
#define I2C_PageSize           8			/* AT24C02???8??? */
#define I2C_Channel			I2C1
uint16_t EEPROM_ADDRESS;

/*
 * ???:I2C_GPIO_Config
 * ??  :I2C1 I/O??
 * ??  :?
 * ??  :?
 * ??  :????
 */
 
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
	/* ??? I2C1 ????? */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
    
  /* PB6-I2C1_SCL?PB7-I2C1_SDA*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ????
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * ???:I2C_Configuration
 * ??  :I2C ??????
 * ??  :?
 * ??  :?
 * ??  :????
 */
static void I2C_Mode_Configu(void)
{
	
I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
I2C_InitStructure.I2C_OwnAddress1 = 0x00;
I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
I2C_InitStructure.I2C_ClockSpeed = 400000 ;

  
  /* ?? I2C1 */
  I2C_Cmd(I2C_Channel, ENABLE);

  /* I2C1 ??? */
  I2C_Init(I2C_Channel, &I2C_InitStructure);

   
}

/*
 * ???:I2C_EE_Init
 * ??  :I2C ??(EEPROM)???
 * ??  :?
 * ??  :?
 * ??  :????
 */
void I2C_EE_Init(void)
{

  I2C_GPIO_Config(); 
 
  I2C_Mode_Configu();

/* ?????i2c_ee.h???????EEPROM?????? */
#ifdef EEPROM_Block0_ADDRESS
  /* ?? EEPROM Block0 ??? */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS  
	/* ?? EEPROM Block1 ??? */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS  
	/* ?? EEPROM Block2 ??? */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS  
	/* ?? EEPROM Block3 ??? */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}

/*
 * ???:I2C_EE_BufferWrite
 * ??  :??????????I2C EEPROM?
 * ??  :-pBuffer ?????
 *         -WriteAddr ?????EEPROM???
 *         -NumByteToWrite ???EEPROM????
 * ??  :?
 * ??  :?
 * ??  :????
 */
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}

/*
 * ???:I2C_EE_ByteWrite
 * ??  :??????I2C EEPROM?
 * ??  :-pBuffer ?????
 *         -WriteAddr ?????EEPROM??? 
 * ??  :?
 * ??  :?
 * ??  :????
 */
void I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C_Channel, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_MODE_SELECT));  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_Channel, 0xA0, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C_Channel, WriteAddr);
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  I2C_SendData(I2C_Channel, *pBuffer); 
   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_Channel, ENABLE);
}

/*
 * ???:I2C_EE_PageWrite
 * ??  :?EEPROM??????????????,?????????
 *         ????EEPROM?????AT24C02???8????
 * ??  :-pBuffer ?????
 *         -WriteAddr ?????EEPROM??? 
 *         -NumByteToWrite ???EEPROM????
 * ??  :?
 * ??  :?
 * ??  :????
 */
void I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
    while(I2C_GetFlagStatus(I2C_Channel, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
    
  /* Send START condition */
  I2C_GenerateSTART(I2C_Channel, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_MODE_SELECT)); 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_Channel, 0xA0, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(I2C_Channel, WriteAddr);  

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C_Channel, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_Channel, ENABLE);
}


/*
 * ???:I2C_EE_BufferRead
 * ??  :?EEPROM????????? 
 * ??  :-pBuffer ???EEPROM????????????
 *         -WriteAddr ?????EEPROM???? 
 *         -NumByteToWrite ??EEPROM???????
 * ??  :?
 * ??  :?
 * ??  :????
 */
void I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  //*((u8 *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(I2C_Channel, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
    
    
  /* Send START condition */
  I2C_GenerateSTART(I2C_Channel, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_Channel, 0xA0, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C_Channel, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C_Channel, ReadAddr);  

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C_Channel, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C_Channel, 0xA0, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C_Channel, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C_Channel, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C_Channel, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C_Channel);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C_Channel, ENABLE);
}


/*
 * ???:I2C_EE_WaitEepromStandbyState
 * ??  :Wait for EEPROM Standby state 
 * ??  :?
 * ??  :?
 * ??  :?
 * ??  : 
 */
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C_Channel, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C_Channel, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_Channel, 0xA0, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C_Channel, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C_Channel, I2C_FLAG_AF);
    /* STOP condition */    
    I2C_GenerateSTOP(I2C_Channel, ENABLE); 
}