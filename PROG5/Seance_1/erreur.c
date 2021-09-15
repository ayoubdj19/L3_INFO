#include <stdio.h>

int main()
{
  int i,n,q,r;

  n = 17;
  q = r = 0;
  for (i=n;i;i--)
    {
      q = q + 1 / (i * (i - 1));
      r = r + 1 % (i * (i - 1));
    }
  printf("Somme des quotients : %d\n",q);
  printf("Somme des restes : %d\n",r);
  return 0;
}
