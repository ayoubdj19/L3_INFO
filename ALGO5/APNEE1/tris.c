#include <stdlib.h>
#include <stdio.h>

#define NMAX 50000		/* peut etre modifie si necessaire */



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
  int i,j,f;
  int Clef;

  f = 0;

  for(i=1; i < n; i++) {

    Clef = t[i];

    j = i - 1;

    /* Decalage des valeurs du tableau */
    while((j >= 0) && (t[j] > Clef)) {
      f++;
      t[j+1] = t[j];
      j = j - 1;
    }

    /* insertion de la clef */
    t[j+1] = Clef;
  }
  /* Affichage du nombre de comparaison */
  printf("Valeur de f = %d\n", f);
  return f;
}

/*
tri_segmentation
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
void tri_segmentation(int t[], int n) {
  /* A completer */
}



void lancer_mesures() {

  unsigned int germe ;
  char res;
  int T[NMAX];
  int N,X;
  float FMOY;

  printf("Valeur du germe pour la fonction de tirage aleatoire ? ") ;
  scanf("%d", &germe) ;
  srand(germe) ;

  do {
    printf("Lancer mesures multiples (cf exercice 2) ?\nO = oui | N = non\n");
    scanf("%c", &res);
  } while(res != 'O' && res != 'o' && res != 'N' && res != 'n');

  do {
    printf("Valeur de N ? ") ;
    scanf("%d", &N) ;
  } while (N<1 || N > NMAX) ;

  if(res == 'N' || res == 'n') {
    generation_aleatoire(T, N) ; /* initialisation du tableau T */
    tri_insertion(T, N) ;	/* tri de T */
  } else {
    do {
      printf("Valeur de X ? ") ;
      scanf("%d", &X) ;
    } while (X<1 || X > NMAX) ;

    FMOY = 0;

    for(int i = 0; i < X; i++) {
      generation_aleatoire(T, N) ; /* initialisation du tableau T */
      FMOY += tri_insertion(T, N) ;	/* tri de T */
    }
    FMOY = FMOY/X;
    printf("On obtient un Fmoy = %f\n", FMOY);
  }

}
