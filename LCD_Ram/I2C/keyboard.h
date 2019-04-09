#include "stm32f10x.h"
#include "stdio.h"
#include "stdbool.h"


#define ESC		0x76
#define BKSP 	0x66
#define ENTER	0x5A
#define CAP		0x58
#define SPACE	0x29
#define LSHIFT	0x12
#define LCTRL		0x14
#define F1		0x05
#define F2 		0x06
#define F3		0x04
#define NewLine 0x0A

void _send(uint8_t);
void _SendString(char*);
	
void NVIC_Configuration(void);
//void SetDefaultLed();
void EnableUart_2(void);