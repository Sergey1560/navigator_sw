#include "main.h"
#include "usart.h"
#include <stdio.h>
#include "exti.h"

volatile uint32_t sys_timer;
volatile uint16_t uart_cmd;

static void send_counter(void);
static void systick_init(void);

int main(void){
	
	RCC_init();

	systick_init();
	tim14_init();   

	uart_init(USART1,9600);
	uart_enable_irq(USART1);
	
	send_counter();
	
	exti_init();

	while(1){
		
		if(uart_cmd == 0x53){ //S
			send_counter();
			uart_cmd = 0;
			reset_counter();
		}

		if(uart_cmd == 0x52){ //R
			send_counter();
			uart_cmd = 0;
		}
		};
}


static void send_counter(void){
	uint8_t	buf[7];

	sprintf(buf,"%04X",get_counter());
	buf[4] = 0x0D;
	buf[5] = 0x0A;
	DEBUG("Reply [%d]: 0x%0X:0x%0X:0x%0X:0x%0X:0x%0X:0x%0X",get_counter(),buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	uart_send_bin(USART1,buf,6);
	buf[4] = 0;
	DEBUG("String: %s",buf);
};


void USART1_IRQHandler(void){
	uint8_t data;
	static uint32_t last_time = 0;


	if(USART1->ISR & USART_ISR_RXNE_RXFNE){
		data = USART1->RDR;
		
		DEBUG("[%d]Get char %c (0x%0X)",sys_timer-last_time,(char)data,data);
		last_time = sys_timer;

		if((data == 0x53)||(data == 0x52)){// S or R
 			uart_cmd = data;
		}
	}
}

void TIM14_IRQHandler(void){
	TIM14->SR &= ~TIM_SR_UIF;
	
}


void systick_init(void){
	uint32_t TimerTick =  SystemCoreClock/1000-1;
 
	sys_timer = 0;

 	SysTick->LOAD=TimerTick;	// Загрузка значения
	SysTick->VAL=TimerTick;		// Обнуляем таймеры и флаги. Записью, помните? 
 
 	SysTick->CTRL=	SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


void SysTick_Handler(void){
	sys_timer++;
}