#ifndef EXTI_H
#define EXTI_H

#include "common_defs.h"




void exti_init(void);
uint16_t get_counter(void);
void reset_counter(void);


#endif