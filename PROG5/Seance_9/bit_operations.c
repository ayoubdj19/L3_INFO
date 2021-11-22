#include "bit_operations.h"


int get_bit(int32_t num, int nbbits){
  return (num >> nbbits) & 1;
}

int32_t set_bit(int32_t num, int bits){
  return bits;
}

int32_t clr_bit(int32_t num, int bits){
  return num - bits;
}
