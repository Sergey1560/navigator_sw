#include "timer.h"
#include "exti.h"

void tim14_init(void){
	RCC->APBENR2 |= RCC_APBENR2_TIM14EN;

    TIM14->PSC = (64000-1);
    TIM14->CNT = 0;
    TIM14->ARR = 1000-1;

    TIM14->EGR |= TIM_EGR_UG;
    while(!(TIM14->SR & TIM_SR_UIF)){};
    TIM14->SR = 0;

	TIM14->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM14_IRQn); 

	TIM14->CR1 |= TIM_CR1_CEN;
}


void TIM14_IRQHandler(void){
	TIM14->SR &= ~TIM_SR_UIF;
	DEBUG("ODO: %d",get_counter());
	reset_counter();
}
