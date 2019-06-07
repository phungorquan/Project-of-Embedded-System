#include "stm32f10x.h"
#define WRMR        1
#define WRITE       2
#define READ        3 
#define ByteMode    0x00
#define Sequential  0x40

// SPI Ram init
void SPI1_RAM_Init(void);

// SetMode Send 1 by 1 or an Array (Sequential)
void SetMode(char);

// Send 8b
uint8_t Send_8b(uint8_t);

// Write And Read for 1 by 1 Mode
void WriteByte(uint32_t , uint8_t);
uint8_t ReadByte(uint32_t );

// Write And Read flr Sequential Mode
void Transfer_An_Array(char *, int );
void WriteArray(uint32_t , char *, uint16_t);
void ReadArray(uint32_t ,char *, uint16_t );

