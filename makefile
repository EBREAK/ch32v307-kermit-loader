CROSS_COMPILE ?= riscv32-qingkev4f-elf-
CC = $(CROSS_COMPILE)gcc
OD = $(CROSS_COMPILE)objdump
OC = $(CROSS_COMPILE)objcopy
SZ = $(CROSS_COMPILE)size

FW_NAME ?= ch32v307-kermit-loader

CH32V307_SDK ?= ./EVT/EXAM/

#LINK_SCRIPT ?= $(CH32V307_SDK)/SRC/Ld/Link.ld
LINK_SCRIPT ?= Link.ld

#STARTUP_SCRIPT ?= $(CH32V307_SDK)/SRC/Startup/startup_ch32v30x_D8C.S
STARTUP_SCRIPT ?= startup_ch32v30x_D8C.S

#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_bkp.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_rng.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_opa.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_can.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_rtc.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_crc.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_iwdg.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_tim.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_adc.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_i2c.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_eth.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_exti.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_dac.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_flash.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_sdio.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_dma.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_fsmc.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_dvp.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_wwdg.c \
#$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_spi.c \

SRCS += \
	$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_dbgmcu.c \
	$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_gpio.c \
	$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_pwr.c \
	$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_misc.c \
	$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_usart.c \
	$(CH32V307_SDK)/SRC/Peripheral/src/ch32v30x_rcc.c \
	$(CH32V307_SDK)/SRC/Debug/debug.c \
	$(CH32V307_SDK)/SRC/Core/core_riscv.c \

INCS += \
	-I $(CH32V307_SDK)/SRC/Core/ \
	-I $(CH32V307_SDK)/SRC/Debug/ \
	-I $(CH32V307_SDK)/SRC/Peripheral/inc/ \

CFLAGS += \
	-pipe \
	-Wall -Wextra \
	-Wno-unused-variable \
	-Wno-unused-parameter \
	-Wno-pointer-to-int-cast \
	-Wno-discarded-qualifiers \
	-Xlinker --gc-sections \
	-march=rv32imacf_zicsr_zifencei -mabi=ilp32 \
	-Os -ggdb \
	--specs=picolibc.specs \
	-nostartfiles \
	-T $(LINK_SCRIPT) \

SRCS += \
	system_ch32v30x.c \
	ch32v30x_it.c \
	usb_desc.c \
	ch32v30x_usbfs_device.c  \
	main.c \

INCS += \
	-I. \

CFLAGS += \
	-DCH32V30x_D8C \

all:
	$(CC) $(CFLAGS) $(STARTUP_SCRIPT) $(INCS) $(SRCS) -o $(FW_NAME).elf
	$(OD) -l -F -S -d $(FW_NAME).elf > $(FW_NAME).dis
	$(OC) -O ihex $(FW_NAME).elf $(FW_NAME).hex
	$(OC) -O binary $(FW_NAME).elf $(FW_NAME).bin
	$(SZ) $(FW_NAME).elf

patch:
	sed -i -e 's/ptrdiff_t/int32_t/g' $(CH32V307_SDK)/SRC/Debug/debug.c

flash:
	wlink flash $(FW_NAME).hex
