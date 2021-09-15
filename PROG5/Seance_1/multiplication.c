int main()
{
  int x,y,u,v,a;

  x = 73;
  y = 29;
  a = 0;
  u = x;
  v = y;

  while (v!=0)
    {
      if (v%2 != 0)
          a = a + u;
      u = u*2;
      v = v/2;
    }

  return 0;
}
