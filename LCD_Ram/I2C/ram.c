#include "ram.h"

GPIO_InitTypeDef GPIO_InitStructkhac; 
SPI_InitTypeDef SPI_InitDef;
	
//void SetA1A0INPUT()
//	{
//		GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//		
//	}

void PeripheralInit_SPI1_Master(){
	
    //GPIO_InitTypeDef GPIO_InitDef;
    //SPI_InitTypeDef SPI_InitDef;
 
    // initialize init structs
    //GPIO_StructInit(&GPIO_InitDef);
    //SPI_StructInit(&SPI_InitDef);
 
    // initialize clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA , ENABLE);
 
    // do not initialize A4/SS because a software SS will be used
 
    // initialize A5/SCK alternate function push-pull (50 MHz)
    GPIO_InitStructkhac.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStructkhac.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructkhac.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructkhac);
 
    // initialize A6/MISO input pull-up (50MHz)
    GPIO_InitStructkhac.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructkhac.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructkhac.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructkhac);
		
		GPIO_InitStructkhac.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructkhac.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructkhac.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructkhac);
 
 
    // initialize SPI master
    // for slave, no need to define SPI_BaudRatePrescaler
    SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitDef.SPI_Mode = SPI_Mode_Master;
    SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;     // 8-bit transactions
    SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;    // MSB first
    SPI_InitDef.SPI_CPOL = SPI_CPOL_Low;            // CPOL = 0, clock idle low
    SPI_InitDef.SPI_CPHA = SPI_CPHA_1Edge;          // CPHA = 1
    SPI_InitDef.SPI_NSS = SPI_NSS_Soft;             // use software SS
    SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // APB2 72/64 = 1.125 MHz
    // SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // APB2 72/256 = 0.28 MHz
    // SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // APB2 72/16 = 4.5 MHz
   // SPI_InitDef.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitDef);
    SPI_Cmd(SPI1, ENABLE);
 
}


uint8_t transfer_8b_SPI1_Master(uint8_t outByte){
 
    // Approach 1, from Brown's book
    // SPI_I2S_SendData(SPI1, outByte); // send
    // while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    // return SPI_I2S_ReceiveData(SPI1); // read received
 
    // Approach 2,
    // from http://www.lxtronic.com/index.php/basic-spi-simple-read-write
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI1, outByte);                                                        // send
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    return SPI_I2S_ReceiveData(SPI1);                                                   // read received
}


void SetMode(char Mode) {                       // Select for single or multiple byte transfer
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);

  transfer_8b_SPI1_Master(WRMR);                           // command to write to mode register
  transfer_8b_SPI1_Master(Mode);                           // set for sequential mode
  
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

void WriteByte(uint32_t address, uint8_t data_byte) {
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);                        // set SPI slave select LOW;
   transfer_8b_SPI1_Master(WRITE);                           // send WRITE command to the memory chip
   transfer_8b_SPI1_Master((uint8_t)(address >> 16));           // send high byte of address
   transfer_8b_SPI1_Master((uint8_t)(address >> 8));            // send middle byte of address
   transfer_8b_SPI1_Master((uint8_t)address);                   // send low byte of address
   transfer_8b_SPI1_Master(data_byte);                       // write the data to the memory location
  GPIO_SetBits(GPIOA, GPIO_Pin_4);                        //set SPI slave select HIGH
}

uint8_t ReadByte(uint32_t address) {
  char read_byte;
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);                        // set SPI slave select LOW;
  transfer_8b_SPI1_Master(READ);                            // send READ command to memory chip
  transfer_8b_SPI1_Master((uint8_t)(address >> 16));           // send high byte of address
  transfer_8b_SPI1_Master((uint8_t)(address >> 8));            // send middle byte of address
  transfer_8b_SPI1_Master((uint8_t)address);                   // send low byte of address
  read_byte = transfer_8b_SPI1_Master(0x00);                // read the byte at that address
  GPIO_SetBits(GPIOA, GPIO_Pin_4);                         // set SPI slave select HIGH;
  return read_byte;                              // send data back to the calling function
}

void transfer(uint8_t *buf, int count) {
    uint8_t *p = buf;
    uint8_t out;
		uint8_t in;
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
	
void WriteArray(uint32_t address, uint8_t *data, uint16_t big) {
  //uint16_t i = 0;                                 // loop counter
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);                         // start new command sequence
  transfer_8b_SPI1_Master(WRITE);                            // send WRITE command
  transfer_8b_SPI1_Master((uint8_t)(address >> 16));            // send high byte of address
  transfer_8b_SPI1_Master((uint8_t)(address >> 8));             // send middle byte of address
  transfer_8b_SPI1_Master((uint8_t)address);                    // send low byte of address
  transfer(data, big);                        // transfer an array of data => needs array name & size
  GPIO_SetBits(GPIOA, GPIO_Pin_4);                            // set SPI slave select HIGH
}

void ReadArray(uint32_t address,uint8_t *data, uint16_t big) {
  uint16_t i = 0;                                 // loop counter
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);                           // start new command sequence
  transfer_8b_SPI1_Master(READ);                             // send READ command
  transfer_8b_SPI1_Master((uint8_t)(address >> 16));            // send high byte of address
  transfer_8b_SPI1_Master((uint8_t)(address >> 8));             // send middle byte of address
  transfer_8b_SPI1_Master((uint8_t)address);                    // send low byte of address
  for (i = 0; i < big; i++) {
    data[i] = transfer_8b_SPI1_Master(0x00);                 // read the data byte
  }
  GPIO_SetBits(GPIOA, GPIO_Pin_4);                        // set SPI slave select HIGH
}
