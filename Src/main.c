#include "main.h"
#include "usart.h"
#include "tim.h"
#include "odo.h"

volatile uint32_t resume_flag=0;
volatile uint32_t uart_cmd=0;

#ifdef DEBUG_ENABLED
volatile uint32_t sys_timer;
static void systick_init(void);
#endif


int main(void){
	
	RCC_init();
	Delay_Init();

	//check pin state for SWD connection
	if( check_start_pin() ){ 
		while (resume_flag < 1){
			//wait debugger to set flag 1
		}
	}

	#ifdef DISABLE_NRST
	remove_nrst();
	#endif

	tim14_init();

	#ifdef DEBUG_ENABLED
	systick_init();
	#endif

	#ifdef DISABLE_NRST
	usart2_tx_init();
	#endif

	usart1_rx_init();

	while(1){
		if(uart_cmd != 0){
			if(uart_cmd == 0x53){ //S
				send_counter();
				reset_counter();
			}

			if(uart_cmd == 0x52){ //R
				send_counter();
				
			}
			uart_cmd = 0;
			}
		}
}


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



uint8_t check_start_pin(void){
	//PA12

	#ifdef DEBUG_ENABLED
	DEBUG("Startup pause");
	Delay_ms(1000);
	DEBUG("Startup pause done");
	#endif
	
	SYSCFG->CFGR1 &= ~SYSCFG_CFGR1_PA12_RMP;  //No PA12 remap
   	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODE12;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD12;

	if(GPIOA->IDR & GPIO_ODR_OD12){ //Pin up, stop for debug
		DEBUG("Debug mode");
		return 1;
	}
	
	DEBUG("Normal startup");
	return 0;
}


/*
Remap NSRT pin to GPIO
*/
#ifdef DISABLE_NRST
void remove_nrst(void){
	uint32_t opt_byte = FLASH->OPTR;
	
	if( ((opt_byte >> NRST_MODE_Pos) & NRST_MODE_MASK) != NRST_MODE_GPIO_MASK ){

		//Unlock Flash
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;

		//Unlock optbyte
		FLASH->OPTKEYR = 0x08192A3B;
		FLASH->OPTKEYR = 0x4C5D6E7F;

		//Wait flash busy
		while(FLASH->SR & FLASH_SR_BSY1){};
		
		//opt_byte = FLASH->OPTR;
		opt_byte &= ~(NRST_MODE_MASK << NRST_MODE_Pos);
		opt_byte |= (NRST_MODE_GPIO_MASK << NRST_MODE_Pos);
		FLASH->OPTR = opt_byte;

		//Wait flash busy
		while(FLASH->SR & FLASH_SR_BSY1){};

		FLASH->CR |= FLASH_CR_OPTSTRT;

		//Wait flash busy
		while(FLASH->SR & FLASH_SR_BSY1){};
	}
}
#endif



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
