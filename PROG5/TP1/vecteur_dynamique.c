#include "vecteur_dynamique.h"
#include <stdlib.h>

vecteur allouer_vecteur(int taille) {
  vecteur v = malloc(sizeof(vecteur));
  v->taille = taille;
  v->donnees = malloc(sizeof(double)*taille);
  if(v->donnees == NULL){
    v->taille = 0;
    return v;
  } else {
    return v;
  }
}

void liberer_vecteur(vecteur v) {
  free(v->donnees);
  free(v);
}

int est_vecteur_invalide(vecteur v) {
  return (v == NULL && v->taille == 0 && v->donnees == NULL);
}

double *acces_vecteur(vecteur v, int i) {
  if(i < 0){
    return NULL;
  } else if(i < v->taille){
    return &v->donnees[i];
  } else {
     double *newdonnees = (double *)realloc(v->donnees, sizeof(double)*i+1);
    if(v->donnees == NULL){
      return NULL;
    } else {
      v->taille = i+1;
      v->donnees = newdonnees;
      return &v->donnees[i];
    }
  }
}

int taille_vecteur(vecteur v) {
  return v->taille;
}
