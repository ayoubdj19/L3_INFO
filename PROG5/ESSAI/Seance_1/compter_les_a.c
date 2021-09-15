#include <stdio.h>
#include "compteur.h"
#include "texte.h"

int main()
{
  compteur c;

  c=nouveau_compteur();
  demarrer();

  while (!fin())
    {
      if (courant()=='A')
          incrementer(c);
      avancer();
    }

  printf("%d\n",valeur(c));
  return 0;
}
