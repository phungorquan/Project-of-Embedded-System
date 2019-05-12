#include "ram.h"

// Mode From Arduino Code
void SPI1_RAM_Init()
{
		GPIO_InitTypeDef GPIO_InitStruct; 
		SPI_InitTypeDef SPI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA , ENABLE);
 
    // initialize A5/SCK alternate function push-pull (50 MHz)
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
 
    // initialize A6/MISO input pull-up (50MHz)
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		// SS , If Ram don't RUN , try change the SS pin to the other
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
 
 
    // Initialize SPI master
    // for slave, no need to define SPI_BaudRatePrescaler
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;     // 8-bit transactions
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;    // MSB first
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;            // CPOL = 0, clock idle low
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;          // CPHA = 1
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;             // use software SS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // APB2 72/4 = 18 MHz ~ 20MHZ of RAM
   // SPI_InitDef.SPI_CRCPolynomial = 7;
    
		// Enable SPI
		SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
 
}


void SetMode(char Mode) {                  // Select for single or multiple byte transfer
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);

  Send_8b(WRMR);                           // Command to write to mode register
  Send_8b(Mode);                           // Send mode
  
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

uint8_t Send_8b(uint8_t outByte){
 
		// Send
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI1, outByte);   
	 
		// Read received
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    return SPI_I2S_ReceiveData(SPI1);                                                  
}

void WriteByte(uint32_t address, uint8_t data_byte) {
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);                       // set SPI slave select LOW;
   
	 Send_8b(WRITE);                           		// send WRITE command to the memory chip
   Send_8b((uint8_t)(address >> 16));           // send high byte of address
   Send_8b((uint8_t)(address >> 8));            // send middle byte of address
   Send_8b((uint8_t)address);                   // send low byte of address
   Send_8b(data_byte);                      		// write the data to the memory location
 
	GPIO_SetBits(GPIOA, GPIO_Pin_4);                        //set SPI slave select HIGH
}

uint8_t ReadByte(uint32_t address) {
  char read_byte;
  
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);                      // set SPI slave select LOW;
  
	Send_8b(READ);                            	// send READ command to memory chip
  Send_8b((uint8_t)(address >> 16));           // send high byte of address
  Send_8b((uint8_t)(address >> 8));            // send middle byte of address
  Send_8b((uint8_t)address);                   // send low byte of address
  read_byte = Send_8b(0x00);               		 // read the byte at that address
  
	GPIO_SetBits(GPIOA, GPIO_Pin_4);                        // set SPI slave select HIGH;
  return read_byte;                              // send data back to the calling function
}


void Transfer_An_Array(char *buf, int count) {				// Transfer an Array
    char *p = buf;
    char out;
		char in;
	if (count == 0) return;
     SPI1->DR  = *p;
    while (--count > 0) {
      out = *(p + 1);
      while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
      in = SPI1->DR;
      SPI1->DR = out;
      *p++ = in;
    }
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    *p = SPI1->DR;
  }
	
void WriteArray(uint32_t address, char *data, uint16_t big) {

  GPIO_ResetBits(GPIOA, GPIO_Pin_4);                         // start new command sequence
  
	Send_8b(WRITE);                           	 	// send WRITE command
  Send_8b((uint8_t)(address >> 16));            // send high byte of address
  Send_8b((uint8_t)(address >> 8));             // send middle byte of address
  Send_8b((uint8_t)address);                    // send low byte of address
  Transfer_An_Array(data, big);                        	// transfer an array of data => needs array name & size
  
	GPIO_SetBits(GPIOA, GPIO_Pin_4);                           // set SPI slave select HIGH
}

void ReadArray(uint32_t address,char *data, uint16_t big) {
  uint16_t i = 0;                           
  
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);                         // start new command sequence
  
	Send_8b(READ);                             		// send READ command
  Send_8b((uint8_t)(address >> 16));            // send high byte of address
  Send_8b((uint8_t)(address >> 8));             // send middle byte of address
  Send_8b((uint8_t)address);                    // send low byte of address
  
	for (i = 0; i < big; i++) 
    data[i] = Send_8b(0x00);                 		// read the data byte
  
  
	GPIO_SetBits(GPIOA, GPIO_Pin_4);                        // set SPI slave select HIGH
}
