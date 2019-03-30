//23LC1024 setup

#ifndef _23LC1024_
#define _23LC1024_

#include "main.h"
#include "setup.h"
#include <string.h>


//SRAM opcodes
#define RDSR 5
#define WRSR 1
#define READ 3
#define WRITE 2

// SRAM modes//23LC1024 SPI Commands
#define S_READ		0x03
#define S_WRITE		0x02
#define S_EDIO		0x3B
#define S_EQIO		0x38
#define S_RSTIO		0xFF
#define S_RDMR		0x05
#define S_WRMR		0x01

//SPI RAM Size
#define S_SIZE		0x1FFFF

//A simple typedef enumeration of the access modes
typedef enum{
	Single,
	Dual,
	Quad
} S23_Access;

//SPI RAM Initialization
void S23LC_Init(void);

//Data writing functions
void S23_WByte(uint32_t, uint8_t);
void S23_WWord(uint32_t, uint16_t);
void S23_WDWord(uint32_t, uint32_t);
void S23_WFloat(uint32_t, float);
void WriteBuffer(char buffer[]);

//Speed testing function (using data writes)
void S23_SpeedTest(void);

//Data reading functions
uint8_t S23_RByte(uint32_t);
uint16_t S23_RWord(uint32_t);
uint32_t S23_RDWord(uint32_t);
float S23_RFloat(uint32_t);

//SPI RAM Setup functions
void S23_IOAccess(S23_Access);
void S23_WriteModeReg(uint8_t);
uint8_t S23_ReadModeReg(void);
#endif
