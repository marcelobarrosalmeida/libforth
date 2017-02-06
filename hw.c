#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

#include "forth.h"
#include "hw.h"
#include "util_cbuf.h"

#define HW_CBUF_SIZE 64

extern UART_HandleTypeDef huart3;

util_cbuf_t io_cb;
util_cbuf_data_t io_buffer[HW_CBUF_SIZE];

void USART3_IRQHandler(void)
{
    uint8_t c;
    uint32_t sr;
    USART_TypeDef *h = huart3.Instance;

    // read status register and handle any remaining error 
    // (read SR and DF for such action)
    sr = USART3->SR;
    while(sr & (USART_SR_ORE | USART_SR_PE | USART_SR_FE | USART_SR_NE))
    {
        sr = h->SR;
        c = h->DR;
    }

    if(sr & USART_SR_RXNE)
    {
        c = h->DR;
        if(util_cbuf_put(&io_cb, c) == UTIL_CBUF_FULL)
        {
          // some action here 
        }
    }
}

static void uart3_send_byte(uint8_t c)
{
	USART_TypeDef *h = huart3.Instance;
    
    while( !(h->SR & USART_SR_TXE)) {}

    h->DR = c;
}

int hw_fgetc ( FILE * stream )
{
    return EOF;
}

int hw_fputc ( int character, FILE * stream )
{
	uart3_send_byte((uint8_t) character);
    return 1;
}

int hw_fputs ( const char * str, FILE * stream )
{
	int n = 0;
	while(str[n] != '\0')
		hw_fputc(str[n++],stream);

	return n;
}

FILE * hw_fopen ( const char * filename, const char * mode )
{
    return (FILE *) 0;
}

int hw_fget_word ( FILE * stream, char * word, int *n)
{
    util_cbuf_data_t c;
    char *p = word;
    int m = 0;
    // remove spaces
    do
    {
    	if(util_cbuf_get(&io_cb, &c) == UTIL_CBUF_EMPTY)
    		continue;

    	if(c == ' ' || c == '\n' || c == '\r' || c == '\t')
    	{
    		m++;
    		continue;
    	}
    	else
    		break;
    } while(1);

    // get at most MAXIMUM_WORD_LENGTH-1 characters
    p[m++] = (char) c;
    while(m < (MAXIMUM_WORD_LENGTH-1))
    {
    	if(util_cbuf_get(&io_cb, &c) == UTIL_CBUF_EMPTY)
    		continue;

    	if(c == ' ' || c == '\n' || c == '\r' || c == '\t')
    		break;
    	p[m++] = (char) c;
    }
   
    p[m] = '\0';
    *n = m;

    return m;
}

int hw_sget_word (const char * s,  char * word, int *n)
{
	const char *ps = s;
    char *pw = word;
    int m = 0, k = 0;

    // remove spaces
    do
    {
    	if(ps[m] == '\0')
    		return EOF;
    	if(ps[m] == ' ' || ps[m] == '\n' || ps[m] == '\r' || ps[m] == '\t')
    	{
    		m++;
    		continue;
    	}
    	else
    		break;
    } while(1);


    // get at most MAXIMUM_WORD_LENGTH-1 characters
    pw[k++] = ps[m++];

    while(m < (MAXIMUM_WORD_LENGTH-1))
    {
    	if(ps[m] == '\0' || ps[m] == ' ' || ps[m] == '\n' || ps[m] == '\r' || ps[m] == '\t')
    		break;

    	pw[k++] = ps[m++];
    }

    pw[k] = '\0';
    *n = m;

    return m;
}

void hw_init(void)
{
	util_cbuf_init(&io_cb, io_buffer, HW_CBUF_SIZE);

	// enabling interrupts for errors
    //   (Frame error, noise error, overrun error)
    huart3.Instance->CR3 |= USART_CR3_EIE;
    // enabling interrupt for parity errors and rx
    huart3.Instance->CR1 |= USART_CR1_PEIE | USART_CR1_RXNEIE;

    HAL_NVIC_SetPriority(USART3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);    
}
