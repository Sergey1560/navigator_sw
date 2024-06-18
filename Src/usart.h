#ifndef USART_H
#define USART_H

#include "common_defs.h"



void uart_send_char(USART_TypeDef* usart, char c);
void uart_send_string(USART_TypeDef* usart, char *s);
void uart_send_bin(USART_TypeDef* usart, uint8_t *data, uint16_t len);
void uart_set_baudrate(USART_TypeDef* usart,uint32_t baudrate);
void uart_enable_irq(USART_TypeDef* usart);
void uart_disable_irq(USART_TypeDef* usart);

void uart_init(USART_TypeDef* usart, uint32_t baudrate);
void uart_stop(USART_TypeDef* usart);
void usart1_gpio_init(void);
void usart2_gpio_init(void);
void usart3_gpio_init(void);
void uart4_gpio_init(void);


void uart2_send_dma(uint8_t *data, uint16_t len);
void uart3_send_dma(uint8_t *data, uint16_t len);


#endif