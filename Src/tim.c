#include "tim.h"

/*
PB1 TIM14 CH1
AF 0
*/

volatile uint16_t input_period = 0;
volatile uint32_t pulse_counter = 0;


void tim14_init(void){


	RCC->APBENR2 |= RCC_APBENR2_TIM14EN;
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	
   	GPIOB->MODER &= ~(GPIO_MODER_MODE1);
	GPIOB->MODER |= GPIO_MODER_MODE1_1;

    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL1_Msk);
	GPIOB->AFR[0] |= (0 << GPIO_AFRL_AFSEL1_Pos);

    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD1_0; //Pull up
    
    //CLK 64 000 000
    //1000
    //TIM 64 000
    
    TIM14->PSC = 1000 - 1;
    TIM14->CNT = 0;
    TIM14->ARR = 0xFFFF-1;

    TIM14->CCMR1 |= TIM_CCMR1_CC1S_0;
    TIM14->CCER |= TIM_CCER_CC1E;
    TIM14->EGR |= TIM_EGR_UG;
    while(!(TIM14->SR & TIM_SR_UIF)){};
    TIM14->SR = 0;
	TIM14->DIER |= TIM_DIER_UIE|TIM_DIER_CC1IE;
	
    NVIC_EnableIRQ(TIM14_IRQn); 

	TIM14->CR1 |= TIM_CR1_CEN;
}


void TIM14_IRQHandler(void){
	uint32_t sr = TIM14->SR;
    uint16_t ccr;

    if(sr & TIM_SR_UIF){
        TIM14->SR &= ~TIM_SR_UIF;
        DEBUG("TIM14 freq overload"); //За время считывания счетчика нащелкало больше 16 бит
        input_period = 0xFFFF;
    }

    if(sr & TIM_SR_CC1IF){
        ccr = TIM14->CCR1; //Чтение, чтобы сбросить флаг
        TIM14->CNT = 0;

        if(ccr >= MIN_INPUT_CCR){
            input_period = ccr;
            pulse_counter++;
        }else{
            input_period = 0;
        }
    }
}


uint32_t tim_get_pulse_count(void){

    return pulse_counter;
}

void tim_reset_counter(void){
    pulse_counter = 0;
}