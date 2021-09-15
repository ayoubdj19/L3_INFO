#include <stdio.h>

int main()
{
  int x;

  /* Cette valeur peut etre modifiee avant la compilation
     ou pendant l'execution sous ddd/gdb */
  x = 65;

  while (x!=1)
      if (x%2 == 0)
          x = x/2;
      else
          x = 3*x + 1;

  printf ("%d\n",x);
  return 0;
}
