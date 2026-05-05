#include "ch32v30x_usbfs_device.h"
#include "uart.h"
#include "fifo.h"
#include "kr.h"
#include "early_debug.h"

int main(void)
{
	SystemCoreClockUpdate();
	Delay_Init();
	uart_init();
	debug_puts("CH32V307 KERMIT BOOTLOADER\r\n");
	fifo8_selftest();
	USBFS_RCC_Init();
	USBFS_Device_Init(ENABLE);
	kr_init();
	while (1) {
		uart_task();
		usbfs_task();
	}
}
