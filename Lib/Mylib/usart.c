#include "usart.h"

void uart_send_char(USART_TypeDef* usart, char c){
	while(!(usart->ISR & USART_ISR_TXE_TXFNF)) {};
	usart->TDR  =  c;
};

void uart_send_string(USART_TypeDef* usart, char *s){
	while(*s) {
    	uart_send_char(usart, *s++);
	};
};

void uart_send_bin(USART_TypeDef* usart, uint8_t *data, uint16_t len){
	while(len--) {
		uart_send_char(usart, (char)*data++);
	};
};


void uart_set_baudrate(USART_TypeDef* usart,uint32_t baudrate){
	
	if( (usart == USART1) ){
		usart->BRR = (APB_CLK)/baudrate;
	}else{
		usart->BRR = (APB_CLK)/baudrate;
	};
}

void usart1_rx_init(void){
	USART1->CR1 &= USART_CR1_UE;
	
	/* USART1 AF0  RX:PB7 */
	RCC->APBENR2 |= RCC_APBENR2_USART1EN;
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL7_Msk);
	GPIOB->AFR[0] |= (0 << GPIO_AFRL_AFSEL7_Pos);

	GPIOB->MODER &= ~(GPIO_MODER_MODE7);
	GPIOB->MODER |= GPIO_MODER_MODE7_1;

	uart_set_baudrate(USART1,9600);

	USART1->CR1 = USART_CR1_RE|USART_CR1_RXNEIE_RXFNEIE; 
	USART1->CR1 |= USART_CR1_UE;

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn,7);
}

void usart2_tx_init(void){
	USART2->CR1 &= USART_CR1_UE;
	
	/* USART2 AF1  TX:PA2 */
	RCC->APBENR1 |= RCC_APBENR1_USART2EN;
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Msk);
	GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL2_Pos);

	GPIOA->MODER &= ~(GPIO_MODER_MODE2);
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	
	uart_set_baudrate(USART2,9600);

	USART2->CR1 = USART_CR1_TE; 
	USART2->CR1 |= USART_CR1_UE;
}

