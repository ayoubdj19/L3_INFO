#include "compteur.h"
#include <stdlib.h>

compteur nouveau_compteur()
{
  compteur c;

  c=(compteur) malloc(sizeof(compteur_type));
  mise_a_zero(c);

  return c;
}

void detruis_compteur(compteur c)
{
  free(c);
}

void mise_a_zero(compteur c)
{
  *c=0;
}

void incrementer(compteur c)
{
  (*c)++;
}

compteur_type valeur(compteur c)
{
  return *c;
}
