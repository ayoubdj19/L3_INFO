#ifndef __BIT_OPERATIONS__
#define __BIT_OPERATIONS__

#include <stdint.h>

int get_bit(int32_t num, int nbbits);
int32_t set_bit(int32_t num, int bits);
int32_t clr_bit(int32_t num, int bits);

#endif
