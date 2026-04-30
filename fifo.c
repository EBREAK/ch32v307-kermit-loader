#include "fifo.h"

// TODO: PERFORMANCE

void fifo8_reset(struct fifo8 *fifo)
{
	atomic_fetch_and(&fifo->head, 0);
	atomic_fetch_and(&fifo->tail, 0);
}

bool fifo8_push(struct fifo8 *fifo, uint8_t c)
{
	_Atomic uint32_t curr_tail;
	_Atomic uint32_t next_tail;
	curr_tail = atomic_load(&fifo->tail);
	next_tail = (curr_tail + 1) & fifo->mask;
	if (next_tail == atomic_load(&fifo->head)) {
		return false;
	}
	fifo->buf[curr_tail] = c;
	atomic_thread_fence(memory_order_seq_cst);
	atomic_store(&fifo->tail, next_tail);
	return true;
}

bool fifo8_pop(struct fifo8 *fifo, uint8_t *c)
{
	_Atomic uint32_t curr_head;
	curr_head = atomic_load(&fifo->head);
	if (curr_head == atomic_load(&fifo->tail)) {
		return false;
	}
	*c = fifo->buf[curr_head];
	atomic_thread_fence(memory_order_seq_cst);
	atomic_store(&fifo->head, (curr_head + 1) & fifo->mask);
	return true;
}

uint32_t fifo8_num_used(struct fifo8 *fifo)
{
	return (atomic_load(&fifo->tail) - atomic_load(&fifo->head)) & fifo->mask;
}

uint32_t fifo8_num_free(struct fifo8 *fifo)
{
	return fifo->mask - fifo8_num_used(fifo);
}

void fifo8_selftest(void)
{
	struct fifo8 fifo8_test;
	struct fifo8 *fifo;
	fifo = &fifo8_test;

	fifo8_reset(fifo);
	fifo8_push(fifo, 0x55);
	uint8_t c;
	fifo8_pop(fifo, &c);
}

