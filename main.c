#include "ch32v30x_usbfs_device.h"

int main(void)
{
	SystemCoreClockUpdate();
	Delay_Init();
	USBFS_RCC_Init();
	USBFS_Device_Init(ENABLE);
	while (1)
		;
}
