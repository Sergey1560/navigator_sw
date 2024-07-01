#include "main.h"
#include "usart.h"
//#include <stdio.h>
#include "exti.h"

volatile uint16_t uart_cmd;

static void convert2hex(uint16_t x, char *res);
static void send_counter(void);

#ifdef DEBUG_ENABLED
volatile uint32_t sys_timer;
static void systick_init(void);
#endif

int main(void){
	RCC_init();

	#ifdef DEBUG_ENABLED
	systick_init();
	tim14_init();   
	#endif

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

	//sprintf((char *)buf,"%04X",get_counter());
	convert2hex(get_counter(),(char *)buf);
	buf[4] = 0x0D;
	buf[5] = 0x0A;
	uart_send_bin(USART1,buf,6);
	
	#ifdef DEBUG_ENABLED
	DEBUG("Reply [%d]: 0x%0X:0x%0X:0x%0X:0x%0X:0x%0X:0x%0X",get_counter(),buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	buf[4] = 0;
	DEBUG("String: %s",buf);
	#endif
};


void USART1_IRQHandler(void){
	uint8_t data;
	
	#ifdef DEBUG_ENABLED
	static uint32_t last_time = 0;
	#endif

	if(USART1->ISR & USART_ISR_RXNE_RXFNE){
		data = USART1->RDR;
		
		#ifdef DEBUG_ENABLED
		DEBUG("[%d]Get char %c (0x%0X)",sys_timer-last_time,(char)data,data);
		last_time = sys_timer;
		#endif

		if((data == 0x53)||(data == 0x52)){// S or R
 			uart_cmd = data;
		}
	}
}

#ifdef DEBUG_ENABLED
void systick_init(void){
	uint32_t TimerTick =  SystemCoreClock/1000-1;
 	sys_timer = 0;

 	SysTick->LOAD=TimerTick;	
	SysTick->VAL=TimerTick;
 	SysTick->CTRL=	SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


void SysTick_Handler(void){
	sys_timer++;
}
#endif



static void convert2hex(uint16_t x, char *res){
	res[0] = TO_HEX(((x & 0xF000) >> 12));   
	res[1] = TO_HEX(((x & 0x0F00) >> 8));
	res[2] = TO_HEX(((x & 0x00F0) >> 4));
	res[3] = TO_HEX((x & 0x000F));
}