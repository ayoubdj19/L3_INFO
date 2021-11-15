#include <stdio.h>
#include <stdlib.h>

int read_s(char * filename) {
  int l,c;
  FILE *f = fopen(filename, "r");

  if (f == NULL) {
    return 1;
  }

  fscanf(f, "%d %d", &l, &c);
  printf("l = %d, c = %d\n", l, c);

  int tab[l*c];

  for(int i = 0; i < l; i++){
    for(int j = 0; j < c; j++){
      fscanf(f, "%x", &tab[i*c+j]);
    }
  }

  // Affichage des pixels en hexa
  // for(int k = 0; k < l*c; k++){
  //   printf("%x\n", tab[k]);
  // }

  int res = fclose(f);
  return res;
}

int main(int argc, char *argv[]) {
  int res;
  if(argc != 3){
    printf("ERREUR: Nombre de paramètre de la fonction incorrect (2 paramètres attendus).\n");
    exit(0);
  }

  res = read_s(argv[1]);
  if(res == 1){
    printf("ERREUR\n");
  } else {
    printf("OK\n");
  }
}
