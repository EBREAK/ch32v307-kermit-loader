#pragma once

#include <string.h>
#include "uart.h"

#define debug_emit uart1_emit
#define debug_type uart1_type
#define debug_puts(s)                                \
	{                                            \
		debug_type((uint8_t *)s, strlen(s)); \
	}

#define debug_cr()                                \
	{                                         \
		debug_type((uint8_t *)"\r\n", 2); \
	}
extern void debug_putnhex(int num, int width);
#define debug_puthex(x)                            \
	{                                          \
		debug_putnhex((x), sizeof(x) * 2); \
	}
extern void debug_assert(char *msg, int line, bool expr);
