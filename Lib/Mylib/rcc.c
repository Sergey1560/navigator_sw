#include "rcc.h"

#define PLLM 1
#define PLLN 8
#define PLLR 2
#define PLLSRC 2

void RCC_init(void){
	uint32_t tmp;

	tmp = PWR->CR1;
	tmp &= ~PWR_CR1_VOS;
	tmp |= PWR_CR1_VOS_0;
	PWR->CR1 = tmp;

	while(PWR->SR2 & PWR_SR2_VOSF){};

	tmp = FLASH->ACR;
	tmp &= ~FLASH_ACR_LATENCY;
	tmp |= FLASH_ACR_LATENCY_1;
	FLASH->ACR = tmp;

	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY)){};

	RCC->CR &= ~(RCC_CR_PLLON);
	while(RCC->CR & RCC_CR_PLLRDY){};

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM|RCC_PLLCFGR_PLLN|RCC_PLLCFGR_PLLR|RCC_PLLCFGR_PLLSRC);
	RCC->PLLCFGR |= ((PLLM - 1) << RCC_PLLCFGR_PLLM_Pos)|\
					(PLLN << RCC_PLLCFGR_PLLN_Pos)|\
					((PLLR - 1) << RCC_PLLCFGR_PLLR_Pos)|\
					(PLLSRC << RCC_PLLCFGR_PLLSRC_Pos);

	RCC->CR |= RCC_CR_PLLON;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

	while((RCC->CR & RCC_CR_PLLRDY) == 0){};

	tmp = RCC->CFGR;
	tmp &= ~(RCC_CFGR_SW);
	tmp |= RCC_CFGR_SW_1;
	RCC->CFGR = tmp;
	
	while((RCC->CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_1)) {};

	SystemCoreClockUpdate();

	DEBUG("SystemCoreClock %d",SystemCoreClock);
}
