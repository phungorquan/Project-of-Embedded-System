#include "stm32f10x.h"
#define WRMR        1
#define WRITE       2
#define READ        3 
#define ByteMode    0x00
#define Sequential  0x40

void PeripheralInit_SPI1_Master(void);
uint8_t transfer_8b_SPI1_Master(uint8_t);
void SetMode(char);
void WriteByte(uint32_t , uint8_t);
uint8_t ReadByte(uint32_t );
void transfer(uint8_t *, int );
void WriteArray(uint32_t , uint8_t *, uint16_t);
void ReadArray(uint32_t ,uint8_t *, uint16_t );

//void SetA1A0INPUT(void);