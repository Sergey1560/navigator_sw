#include "usart.h"

void uart_send_char(USART_TypeDef* usart, char c){
	while(!(usart->ISR & USART_ISR_TXE_TXFNF)) {};  //пока бит TXE не стал 1, буфер занят
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

void uart_enable_irq(USART_TypeDef* usart){
	if(usart == USART1) NVIC_EnableIRQ(USART1_IRQn);
	if(usart == USART2) {
		NVIC_EnableIRQ(USART2_IRQn);
		NVIC_SetPriority(USART2_IRQn,7);
	}
	usart->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
};


void uart_disable_irq(USART_TypeDef* usart){
	if(usart == USART1) NVIC_DisableIRQ(USART1_IRQn);
	if(usart == USART2) NVIC_DisableIRQ(USART2_IRQn);
	usart->CR1 &= ~USART_CR1_RXNEIE_RXFNEIE;
};


void uart_init(USART_TypeDef* usart, uint32_t baudrate){
	
	usart->CR1 &= USART_CR1_UE;
	
	//Инициализация пинов и включение тактирования
	//if(usart == USART1) usart1_gpio_init();
	if(usart == USART2) usart2_gpio_init();

	uart_set_baudrate(usart,baudrate);
	usart->CR1 = USART_CR1_TE|USART_CR1_RE; 
	usart->CR1 |= USART_CR1_UE;
};


void uart_stop(USART_TypeDef* usart){
	usart->CR1 = 0;
};


/* USART1  APB2  RX:PA10 TX:PA9 */
// void usart1_gpio_init(void){
// 	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
// 	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
// 	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFRH9|GPIO_AFRH_AFRH10);
// 	GPIOA->AFR[1] |= (7U << GPIO_AFRH_AFRH9_Pos)|(7U << GPIO_AFRH_AFRH10_Pos);

// 	GPIOA->MODER &= ~(GPIO_MODER_MODER9|GPIO_MODER_MODER10);
// 	GPIOA->MODER |= GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1;
	
// 	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR9)|(GPIO_OSPEEDER_OSPEEDR10);
// 	GPIOA->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR9_Pos)|(S_VH << GPIO_OSPEEDER_OSPEEDR10_Pos);
// };

/* USART2  AF1  RX:PA3 TX:PA2 */
void usart2_gpio_init(void){
	RCC->APBENR1 |= RCC_APBENR1_USART2EN;
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Msk|GPIO_AFRL_AFSEL3_Msk);
	GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL2_Pos)|(1 << GPIO_AFRL_AFSEL3_Pos);

	GPIOA->MODER &= ~(GPIO_MODER_MODE2|GPIO_MODER_MODE3);
	GPIOA->MODER |= GPIO_MODER_MODE2_1|GPIO_MODER_MODE3_1;
	
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED2)|(GPIO_OSPEEDR_OSPEED3);
	GPIOA->OSPEEDR |= (S_VH << GPIO_OSPEEDR_OSPEED2_Pos)|(S_VH << GPIO_OSPEEDR_OSPEED3_Pos);
};

