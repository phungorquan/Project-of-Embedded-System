//uart_printf
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
		
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
		
	#include "main.h"
	#include <stdio.h>
	#include <string.h>
	PUTCHAR_PROTOTYPE
#endif
	