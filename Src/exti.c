#include "exti.h"


volatile uint16_t odo_counter;

void exti_init(void){
/*
PA7 - IRQ - Input Pull UP
*/
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODE7;
	
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED7);
	GPIOA->OSPEEDR |= (S_VH << GPIO_OSPEEDR_OSPEED7_Pos);

    GPIOA->OTYPER &= ~GPIO_OTYPER_OT7;

	//Pull UP
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD7;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD7_0;

	EXTI->EXTICR[2] |= EXTI_EXTICR2_EXTI7_0; 
	EXTI->FTSR1 	|= EXTI_FTSR1_FT7;

	NVIC_EnableIRQ (EXTI4_15_IRQn);
	NVIC_SetPriority (EXTI4_15_IRQn, 8);
	//Разрешаем прерывания в периферии
	EXTI->IMR1 |= EXTI_IMR1_IM7;
};

uint16_t get_counter(void){
    return odo_counter;
}

void reset_counter(void){
    odo_counter = 0;
}

void EXTI4_15_IRQHandler(void){
	if(EXTI->FPR1 & EXTI_FPR1_FPIF7){
		EXTI->FPR1 = EXTI_FPR1_FPIF7;
        DEBUG("GET IRQ");
        odo_counter++;
	};

}
