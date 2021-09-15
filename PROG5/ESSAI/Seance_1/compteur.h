#ifndef __COMPTEUR__
#define __COMPTEUR__

typedef int compteur_type;
typedef compteur_type *compteur;

compteur nouveau_compteur();
void detruis_compteur(compteur c);
void mise_a_zero(compteur c);
void incrementer(compteur c);
compteur_type valeur(compteur c);

#endif
