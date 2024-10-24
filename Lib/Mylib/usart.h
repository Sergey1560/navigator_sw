#ifndef USART_H
#define USART_H

#include "common_defs.h"



void uart_send_char(USART_TypeDef* usart, char c);
void uart_send_string(USART_TypeDef* usart, char *s);
void uart_send_bin(USART_TypeDef* usart, uint8_t *data, uint16_t len);
void uart_set_baudrate(USART_TypeDef* usart,uint32_t baudrate);

void usart1_rx_init(void);
void usart2_tx_init(void);

#endif