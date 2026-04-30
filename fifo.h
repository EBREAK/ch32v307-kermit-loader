#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>

struct fifo8 {
	uint8_t *buf;
	uint32_t mask;
	_Atomic uint32_t head;
	_Atomic uint32_t tail;
};

extern void fifo8_reset(struct fifo8 *fifo);
extern bool fifo8_push(struct fifo8 *fifo, uint8_t c);
extern bool fifo8_pop(struct fifo8 *fifo, uint8_t *c);
extern uint32_t fifo8_num_used(struct fifo8 *fifo);
extern uint32_t fifo8_num_free(struct fifo8 *fifo);
extern void fifo8_selftest(void);
