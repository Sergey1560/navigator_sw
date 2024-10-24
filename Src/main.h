#ifndef MAIN_H
#define MAIN_H
#include "common_defs.h"
#include "rcc.h"



#ifdef DISABLE_NRST
#define NRST_MODE_NRST_MASK (uint32_t)(1)
#define NRST_MODE_GPIO_MASK (uint32_t)(2)
#define NRST_MODE_MASK (uint32_t)(3)
#define NRST_MODE_Pos (uint32_t)(27)
void remove_nrst(void);
#endif


uint8_t check_start_pin(void);

#endif

