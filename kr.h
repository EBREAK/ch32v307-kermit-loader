#pragma once

#include "kermit.h"
#include "uart.h"
#include "bootloader.h"
#include "early_debug.h"

extern struct kermit_context kctx_iap;

extern void kr_init(void);
