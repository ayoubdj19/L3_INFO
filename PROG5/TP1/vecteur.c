#include "vecteur.h"
#include <stdlib.h>

vecteur allouer_vecteur(int taille) {
  vecteur v;
  v.taille = taille;
  v.donnees = malloc(sizeof(double) * taille);
  if(v.donnees == NULL){
    v.taille = 0;
  }
  return v;
}

void liberer_vecteur(vecteur v) {
  free(v.donnees);
}

int est_vecteur_invalide(vecteur v) {
  return (v.taille == 0 && v.donnees == NULL);
}

double *acces_vecteur(vecteur v, int i) {
  return &v.donnees[i];
}

int taille_vecteur(vecteur v) {
  return v.taille;
}
