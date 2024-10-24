#ifndef ODO_H
#define ODO_H
#include "common_defs.h"

#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)

void send_counter(void);
void reset_counter(void);

#endif

