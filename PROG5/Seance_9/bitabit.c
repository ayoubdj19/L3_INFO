#include "bit_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TAILLE_BUFFER 128

void aide()
{
  fprintf(stderr,"Aide :\n");
  fprintf(stderr,"Saisir l'une des commandes suivantes\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"a                  : affiche la variable toto en binaire\n");
  fprintf(stderr,"s nombre           : met a 1 le bit 'nombre' de toto\n");
  fprintf(stderr,"c nombre           : met a 0 le bit 'nombre' de toto\n");
  fprintf(stderr,"h                  :   afficher cette aide\n");
  fprintf(stderr,"q                  :   quitter ce programme\n");
}

int main()
{
  char buffer[TAILLE_BUFFER];
  int commande;
  int nombre, i;
  int32_t toto=0;

  aide();
  while (1)
    {
      commande = getchar();
      if (commande == EOF)
          return 0;
      switch (commande)
        {
        case 'a':
          for (i=(sizeof(toto)*8)-1; i>=0; i--)
              printf("%d", get_bit(toto, i));
          printf("\n");
          break;
        case 's':
          scanf ("%d",&nombre);
          toto = set_bit(toto, nombre);
          break;
        case 'c':
          scanf ("%d",&nombre);
          toto = clr_bit(toto, nombre);
          break;
        case 'h':
          aide();
          break;
        case 'q':
          exit(0);
        default:
          fprintf(stderr,"Commande inconnue !\n");
        }
      /* vide ce qu'il reste de la ligne dans le buffer d'entree */
      fgets(buffer,TAILLE_BUFFER,stdin);
    }
  return 0;
}
