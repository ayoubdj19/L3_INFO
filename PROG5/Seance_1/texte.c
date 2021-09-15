#include <stdio.h>
#include <string.h>

#define LONGUEUR_MAX 1024

static char ligne[LONGUEUR_MAX];
static int longueur,rang;

void demarrer()
{
  fgets(ligne,LONGUEUR_MAX,stdin);
  rang=0;
  longueur=strlen(ligne);
}

void avancer()
{
  rang++;
}

char courant()
{
  return ligne[rang];
}

int fin()
{
  return rang==longueur;
}
