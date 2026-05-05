#include "kr.h"

struct kermit_context kctx_iap = KERMIT_INIT(KERMIT_ROLE_RECV);

__aligned(4) uint8_t flash_pbuf[256];
uint16_t flash_pidx = 0;
uint32_t flash_paddr = 0xFFFFFFFF;

void kr_link_send(uint8_t *buf, uint8_t len)
{
	uart1_type(buf, len);
	usb_hid_send(buf, len);
}

static void kermit_fhdr_cb(struct kermit_context *kctx)
{
	char *fn;
	int fnlen;
	fn = (char *)&kctx->buf[KERMIT_DATA];
	fnlen = kermit_decode_buf((uint8_t *)fn, kermit_datalen_get(kctx->buf),
				  (uint8_t *)fn);
	fn[fnlen] = '\0';
	memset(flash_pbuf, 0x00, sizeof(flash_pbuf));
	flash_paddr = APP1_BASE;
	flash_pidx = 0;
	kermit_ack(kctx, kctx->lseqn);
	kctx->lseqn = (kctx->lseqn + 1) & KERMIT_SEQ_MASK;
	return;
}

bool iap_flash_commit(void)
{
	goto done;
done:
	memset(flash_pbuf, 0x00, sizeof(flash_pbuf));
	flash_paddr += sizeof(flash_pbuf);
	flash_pidx = 0;
	return true;
	goto error;
error:
	memset(flash_pbuf, 0x00, sizeof(flash_pbuf));
	return false;
}

bool iap_flash_write(uint8_t c)
{
	if (flash_pidx >= sizeof(flash_pbuf)) {
		if (iap_flash_commit() == false) {
			return false;
		}
	}
	flash_pbuf[flash_pidx] = c;
	flash_pidx += 1;
	return true;
}

static void kermit_data_cb(struct kermit_context *kctx)
{
	uint8_t *wbuf;
	uint8_t idx, len;
	int ret;
	wbuf = &kctx->buf[KERMIT_DATA];
	len = kermit_decode_buf(wbuf, kermit_datalen_get(kctx->buf), wbuf);
	idx = 0;
	while (idx < len) {
		if (iap_flash_write(wbuf[idx]) == false) {
			kermit_error(kctx, "FLASH WRITE FAILED");
			return;
		}
		idx += 1;
	}
	kermit_ack(kctx, kctx->lseqn);
	kctx->lseqn = (kctx->lseqn + 1) & KERMIT_SEQ_MASK;
	return;
}

static void kermit_eof_cb(struct kermit_context *kctx)
{
	if (flash_pidx != 0) {
		if (iap_flash_commit() == false) {
			kermit_error(kctx, "FLASH WRITE FAILED");
			return;
		}
	}
	kermit_ack(kctx, kctx->lseqn);
	kctx->lseqn = (kctx->lseqn + 1) & KERMIT_SEQ_MASK;
	return;
}

static void kermit_break_cb(struct kermit_context *kctx)
{
	kermit_ack(kctx, kctx->lseqn);
	return;
}

static void kermit_error_cb(struct kermit_context *kctx)
{
	char *msg;
	int msglen;
	msg = (char *)&kctx->buf[KERMIT_DATA];
	msglen = kermit_decode_buf(
		(uint8_t *)msg, kermit_datalen_get(kctx->buf), (uint8_t *)msg);
	msg[msglen] = '\0';
	debug_puts("KERMIT ERROR: ");
	debug_puts(msg);
	debug_cr();
	return;
}

void kr_init(void)
{
	struct kermit_context *kctx = &kctx_iap;
	kctx->link_send = kr_link_send;
	kctx->fhdr_cb = kermit_fhdr_cb;
	kctx->data_cb = kermit_data_cb;
	kctx->eof_cb = kermit_eof_cb;
	kctx->break_cb = kermit_break_cb;
	kctx->error_cb = kermit_error_cb;
}
