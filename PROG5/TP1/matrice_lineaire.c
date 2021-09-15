#include "matrice_lineaire.h"
#include <stdlib.h>

matrice allouer_matrice(int l, int c) {
  matrice m;
  m.l = l;
  m.c = c;
  m.donnees = malloc(sizeof(double)*l*c);
  if(m.donnees == NULL){
    m.l = 0;
    m.c = 0;
    return m;
  }
  return m;
}

void liberer_matrice(matrice m) {
  free(m.donnees);
}

int est_matrice_invalide(matrice m) {
  return (m.l == 0 && m.c == 0 && m.donnees == NULL);
}

double *acces_matrice(matrice m, int i, int j) {
  return &m.donnees[i*m.c+j];
}

int nb_lignes_matrice(matrice m) {
  return m.l;
}

int nb_colonnes_matrice(matrice m) {
  return m.c;
}
