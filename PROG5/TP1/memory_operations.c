#include "memory_operations.h"
#include <stdlib.h>

void *my_memcpy(void *dst, const void *src, size_t len) {
  char *dsrc = (char *)src;
  char *ddst = (char *)dst;
  for(int i = 0; i < len; i++){
    ddst[i] = dsrc[i];
  }
  return ddst;
}

void *my_memmove(void *dst, const void *src, size_t len) {

  char *dsrc = (char *)src;
  char *ddst = (char *)dst;

  void *test = NULL;
  do {

  } while(i < len)
  for(int j = 0; j < i; j++){
    ddst[i] = dsrc[i];
  }
  return ddst;
}

int is_little_endian() {
  int n = 1;
  return (*(char*)&n == 1);
}

int reverse_endianess(int value) {
  int resultat = 0;
  return resultat;
}
