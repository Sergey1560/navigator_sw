#ifndef TIM_H
#define TIM_H
#include "common_defs.h"

#define MIN_INPUT_CCR   (uint16_t)190 //Около 150 км/ч

void tim14_init(void);
uint32_t tim_get_pulse_count(void);
void tim_reset_counter(void);

#endif

