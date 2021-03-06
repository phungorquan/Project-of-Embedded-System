#include "stm32f10x.h"

uint8_t Letter[] = {
	
	'/','/','/','/','/'/*F3*/,' '/*F1*/,' '/*F2*/,'/','/','/','/','/','/','/','`','/','/','/','/' /*LSHIFT*/,'/', '/'/*LCRTL*/
	,'Q','1','/','/','/','Z','S','A','W','2','/','/','C','X','D','E','4','3','/','/'
	,' ' /*SPACE*/,'V','F','T','R','5','/','/','N','B','H','G','Y','6','/','/','/','M','J','U'
	,'7','8','/','/','/','K','I','O','0','9','/','/','/','/','L','/','P','-','/','/'
	,'/','/','/','/','='
};

#define ESC		0x76
#define BKSP 	0x66
#define ENTER	0x5A
#define CAP		0x58
#define SPACE	0x29
#define LSHIFT	0x12
#define LCTRL		0x14
#define F1		0x05	// INPUT RAM
#define F2 		0x06	// PRINT INPUT TO LCD
#define F3		0x04	// SET DEFAULT ID & PASS (ID : ABCDE , PASS : 12345)
#define F4		0x0C	// CHANGE ID & PASSWORD
#define F5		0x03	// ACCESS DIRECTLY ID , PASS 
#define NewLine 0x0A

