#include "early_debug.h"

void debug_putnhex(int num, int width)
{
	const char num2hex_lut[] = "0123456789ABCDEF";
	while (width > 0) {
		debug_emit(num2hex_lut[(num >> ((width - 1) * 4)) & 0xF]);
		width -= 1;
	}
}

void debug_assert(char *msg, int line, bool expr)
{
	if (!expr) {
		debug_puts("ASSERT FAILED ");
		debug_puts(msg);
		debug_emit(' ');
		debug_puthex(line);
		debug_cr();
		while (1)
			;
	}
	return;
}
