#pragma once

#include "ch32v30x_conf.h"
#include "fifo.h"

extern bool uart1_tx_rdy(void);
extern void uart1_emit(char c);
extern void uart1_type(uint8_t *buf, int len);

extern struct fifo8 usart1_rxfifo;
extern void uart_init(void);
extern void uart_task(void);
