#include "stm32f10x.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"

// Send UART
void _SendByte(uint8_t);
void _SendString(char*);

// Config Priority UART interrupt
void NVIC_Configuration(void);
// Enable Uart2
void EnableUart_2(void);

// All GPIO will be Config in this function
void GPIO_Setup(void);

// Delay for whole program
void Delay(int);
