#ifndef __UTIL_CBUF__
#define __UTIL_CBUF__

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t util_cbuf_data_t;

#define UTIL_CBUF_INC(v,mv)   ((((v) + 1) >= (mv)) ? 0 : (v) + 1)

typedef enum util_cbuf_status_s
{
    UTIL_CBUF_OK = 0,
    UTIL_CBUF_FULL,
    UTIL_CBUF_EMPTY,
} util_cbuf_status_t;

typedef struct util_cbuf_s
{
	volatile uint16_t head;
	volatile uint16_t tail;
	util_cbuf_data_t *buffer;
	uint16_t size;
} util_cbuf_t;

util_cbuf_status_t util_cbuf_init(util_cbuf_t *cb, util_cbuf_data_t *area, uint16_t size);
util_cbuf_status_t util_cbuf_get(util_cbuf_t *cb, util_cbuf_data_t *c);
util_cbuf_status_t util_cbuf_put(util_cbuf_t *cb, util_cbuf_data_t c);
util_cbuf_status_t util_cbuf_flush(util_cbuf_t *cb);

#ifdef __cplusplus
}
#endif

#endif /* __UTIL_CBUF__ */


