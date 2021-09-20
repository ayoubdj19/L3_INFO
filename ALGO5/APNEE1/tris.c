#include <stdlib.h>
#include <stdio.h>

#define NMAX 50000		/* peut etre modifie si necessaire */

int F;

/*
generation_aleatoire
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : les valeurs t[0..n-1] ont ete initialisees avec n valeurs aleatoires
           prises dans [0,2147483647] selon une loi uniforme.
Pre-condition : le generateur aleatoire doit avoir ete initialise avec "srand"
*/

void generation_aleatoire(int t[], int n) {
  int i ;

  for (i=0 ; i<n ; i++) {
	  t[i] = rand() ;
  } ;
}

/*
generation_aleatoire_non_uniforme
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : les valeurs t[0..n-1] ont ete initialisees avec n valeurs aleatoires
           prises dans [0,2147483647].
Pre-condition : le generateur aleatoire doit avoir ete initialise avec "srand"
*/
void generation_aleatoire_non_uniforme(int t[], int n) {
  int i;
  int x = 0;

  for (i=0; i < n; i++) {
    t[i] = x;
    x = x + ((rand() % 10) - 2);
  }
}

/*
tri_insertion
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
int tri_insertion(int t[], int n) {
  int i,j;
  int Clef;

  F = 0;

  for(i=1; i < n; i++) {

    Clef = t[i];

    j = i - 1;

    /* Decalage des valeurs du tableau */
    while((j >= 0) && (t[j] > Clef)) {
      F++;
      t[j+1] = t[j];
      j = j - 1;
    }

    /* insertion de la clef */
    t[j+1] = Clef;
  }
  /* Affichage du nombre de comparaison */
  // printf("Valeur de f = %d\n", F);
  return F;
}



void swap(int *a, int *b){
  int t = *a;
  *a = *b;
  *b = t;
}

/*
partitionner
Donnees : t : tableau d'entiers de taille > n, p, d, f : entier > 0
p représente la position du premier entier du tableau
d représente la position du dernier entier du tableau
pivot représente la position du pivot du tableau
Resultat : L'indice i de pivot correctement placé dans le tableau
*/
int partitionner(int t[], int p, int d){
  F = 0;
  int i = p;
  while(i < d){
    if(t[i+1] <= t[p]){
      F++;
      i++;
    } else {
      swap(&t[d],&t[i+1]);
      d--;
    }
  }
  swap(&t[p],&t[i]);
  //printf("Valeur de f = %d\n", F);
  return i;
}

void tri_rapide(int t[], int p, int d){
  int pivot;
  if(p < d){
    pivot = partitionner(t,p,d);
    tri_rapide(t,p,pivot-1);
    tri_rapide(t,pivot+1,d);
  }
}

/*
tri_segmentation
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
void tri_segmentation(int t[], int n) {
  tri_rapide(t, 0, n-1);
}

int tri_correct(int t[], int n){
  for(int i = 1; i < n; i++){
    if(t[i-1] > t[i]){
      printf("\n\tTRI INCORRECT\n");
      return 0;
    }
  }
  printf("\n\tTRI OK\n");
  return 1;
}


void lancer_mesures() {

  unsigned int germe;
  int T[NMAX];
  int X,nbN,choix,choixG;
  int *N;
  float FMOY;


  printf("Valeur du germe pour la fonction de tirage aleatoire ? ") ;
  scanf("%d", &germe);
  srand(germe);

  printf("Nombre de taille de tableau différent souhaité ? ");
  scanf("%d", &nbN);

  N = malloc(sizeof(int)*nbN);

  for(int i = 0; i < nbN; i++){
    do {
      printf("Valeur de N ? ");
      scanf("%d", &N[i]) ;
    } while (N[i]<1 || N[i] > NMAX);
  }


  do {
    printf("Valeur de X ? ");
    scanf("%d", &X);
  } while (X<1 || X > NMAX);

  do {
  printf("Choix de l'algorithme de tri :\n\t1)tri_insertion\n\t2)tri_segmentation\n\tEntrer le numéro de l'algorithme souhaité : ");
  scanf("%d", &choix);
  } while (choix < 1 || choix > 2);

  do {
  printf("\nChoix de la fonction de génération des tableaux :\n\t1)generation_aleatoire\n\t2)generation_aleatoire_non_uniforme\n\tEntrer le numéro de l'algorithme souhaité : ");
  scanf("%d", &choixG);
  } while (choix < 1 || choix > 2);

  if(choixG == 2){
    choix+=2;
  }

  FMOY = 0;
  printf("\n--------------------------------------------\n");
  switch (choix) {
    case 1:
      printf("\nVous avez choisi l'algorithme de tri par insertion avec la fonction de génération aléatoire\n");
      for(int k = 0; k < nbN; k++){
        for(int l = 0; l < X; l++) {
          generation_aleatoire(T, N[k]); /* initialisation du tableau T */
          FMOY += tri_insertion(T, N[k]);	/* tri de T */
        }
        FMOY = FMOY/X;
        printf("\n\tRESULTAT POUR N n°%d\nOn obtient un Fmoy = %f\n--------------------------------------------\n", k+1, FMOY);
        FMOY = 0;
      }
      break;

    case 2:
      printf("\nVous avez choisi l'algorithme de tri par segmentation avec la fonction de génération aléatoire\n");
      for(int k = 0; k < nbN; k++){
        for(int l = 0; l < X; l++) {
          generation_aleatoire(T, N[k]); /* initialisation du tableau T */
          tri_segmentation(T, N[k]);	/* tri de T */
          FMOY += F;
        }
        FMOY = FMOY/X;
        printf("\n\tRESULTAT POUR N n°%d\nOn obtient un Fmoy = %f\n--------------------------------------------\n", k+1, FMOY);
        FMOY = 0;
      }
      break;

    case 3:
      printf("\nVous avez choisi l'algorithme de tri par insertion avec la fonction de génération aléatoire non uniforme\n");
      for(int k = 0; k < nbN; k++){
        for(int l = 0; l < X; l++) {
          generation_aleatoire_non_uniforme(T, N[k]); /* initialisation du tableau T */
          FMOY += tri_insertion(T, N[k]);	/* tri de T */
        }
        FMOY = FMOY/X;
        printf("\n\tRESULTAT POUR N n°%d\nOn obtient un Fmoy = %f\n--------------------------------------------\n", k+1, FMOY);
        FMOY = 0;
      }
      break;

    case 4:
      printf("\nVous avez choisi l'algorithme de tri par segmentation avec la fonction de génération aléatoire non uniforme\n");
      for(int k = 0; k < nbN; k++){
        for(int l = 0; l < X; l++) {
          generation_aleatoire_non_uniforme(T, N[k]); /* initialisation du tableau T */
          tri_segmentation(T, N[k]);	/* tri de T */
          FMOY += F;
        }
        FMOY = FMOY/X;
        printf("\n\tRESULTAT POUR N n°%d\nOn obtient un Fmoy = %f\n--------------------------------------------\n", k+1, FMOY);
        FMOY = 0;
      }
      break;
  }

  // Affichage du tableau trié
  /*printf("[");
  for(int a = 0; a < N[0]; a++){
    printf(" %d", T[a]);
  }
  printf("]\n");*/

  // Verification du tri du tableau
  //tri_correct(T,N[0]);
}
