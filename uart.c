#include "uart.h"
#include "kr.h"

#define USART1_RX_FIFO_SIZE 128

volatile uint32_t usart1_rx_bytes = 0;
static uint8_t usart1_rxfifo_buf[USART1_RX_FIFO_SIZE];
struct fifo8 usart1_rxfifo = {
	.buf = &usart1_rxfifo_buf[0],
	.mask = USART1_RX_FIFO_SIZE - 1,
	.head = 0,
	.tail = 0,
};

void USART1_IRQHandler(void) __attribute__((interrupt()));

bool uart1_tx_rdy(void)
{
	if ((USART1->STATR & USART_STATR_TC) != 0) {
		return true;
	}
	return false;
}

void uart1_emit(char c)
{
	while (uart1_tx_rdy() == false)
		;
	USART1->DATAR = c;
}

void uart1_type(uint8_t *buf, int len)
{
	while (len > 0) {
		uart1_emit(buf[0]);
		buf += 1;
		len -= 1;
	}
}

void uart_task(void)
{
	uint8_t c;
	while (fifo8_pop(&usart1_rxfifo, &c) == true) {
		kermit_handle_char(&kctx_iap, c);
		kermit_handle_rxpkt(&kctx_iap);
	}
}

void uart1_init(void)
{
	RCC->APB2PRSTR &= ~(RCC_USART1EN | RCC_IOPARST | RCC_IOPBRST | RCC_AFIORST);
	RCC->APB2PCENR |= (RCC_USART1EN | RCC_IOPAEN | RCC_IOPBEN | RCC_AFIOEN);
	/*
	  UART1 PIN:
	  TX PB15
	  RX PA8
	 */
	AFIO->PCFR1 &= ~(AFIO_PCFR1_USART1_REMAP);
	AFIO->PCFR2 |= AFIO_PCFR2_USART1_REMAP;
	GPIOA->CFGHR &= ~(0xF << 0);
	GPIOA->CFGHR |= 0b0100 << 0;
	GPIOB->CFGHR &= ~(0xF << 28);
	GPIOB->CFGHR |= 0b1011 << 28;
	USART1->CTLR1 = 0;
	USART1->CTLR2 = 0;
	USART1->CTLR3 = 0;
	USART1->CTLR4 = 0;
	USART1->CTLR1 = USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_UE;
	/*
	  CLOCK SRC FREQ: 48M
	  BAUDRATE: 115200
	 */
	USART1->BRR = (26 << 4) | (1 << 0);
	USART1->CTLR1 |= USART_CTLR1_RXNEIE;
	NVIC_EnableIRQ(USART1_IRQn);
}

void uart_init(void)
{
	uart1_init();
}

void USART1_IRQHandler(void)
{
	if ((USART1->STATR & USART_STATR_RXNE) != 0) {
		fifo8_push(&usart1_rxfifo, USART1->DATAR);
		usart1_rx_bytes += 1;
	}
}
