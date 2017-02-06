#include <stdint.h>
#include "util_cbuf.h"

util_cbuf_status_t util_cbuf_init(util_cbuf_t *cb, util_cbuf_data_t *area, uint16_t size)
{
	cb->buffer = area;
	cb->size = size;
	cb->tail = cb->head = 0;

	return UTIL_CBUF_OK;
}

util_cbuf_status_t util_cbuf_flush(util_cbuf_t *cb)
{
	util_cbuf_data_t c;
	
	// pode ser bem simples como
	// cb->tail = cb->head = 0; 

	while(util_cbuf_get(cb,&c) != UTIL_CBUF_EMPTY)
	{}
	
	return UTIL_CBUF_OK;
}

util_cbuf_status_t util_cbuf_get(util_cbuf_t *cb, util_cbuf_data_t *c)
{
	if(cb->tail == cb->head)
		return UTIL_CBUF_EMPTY;

	*c = cb->buffer[cb->tail];
	cb->tail = UTIL_CBUF_INC(cb->tail,cb->size);

	return UTIL_CBUF_OK;
}

util_cbuf_status_t util_cbuf_put(util_cbuf_t *cb, util_cbuf_data_t c)
{
	uint16_t next_head = UTIL_CBUF_INC(cb->head,cb->size);

	if(next_head == cb->tail)
		return UTIL_CBUF_FULL;

	cb->buffer[cb->head] = c;
	cb->head = next_head;

	return UTIL_CBUF_OK;
}


