#include "matrice.h"
#include <stdlib.h>

matrice allouer_matrice(int l, int c) {
  matrice m;
  m.l = l;
  m.c = c;
  m.donnees = malloc(sizeof(double)*l);
  for(int i = 0; i < l; i++){
    m.donnees[i] = malloc(sizeof(double)*c);
  }
  if(m.donnees == NULL){
    m.l = 0;
    m.c = 0;
    return m;
  }
  return m;
}

void liberer_matrice(matrice m) {
  for(int i = 0; i < m.l; i++){
    free(m.donnees[i]);
  }
  free(m.donnees);
}

int est_matrice_invalide(matrice m) {
  return (m.l == 0 && m.c == 0 && m.donnees == NULL);
}

double *acces_matrice(matrice m, int i, int j) {
  return &m.donnees[i][j];
}

int nb_lignes_matrice(matrice m) {
  return m.l;
}

int nb_colonnes_matrice(matrice m) {
  return m.c;
}
