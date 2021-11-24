#include <stdio.h>

#include "arbrebin.h"
#include "assert.h"
#include "bfile.h"

void Decoder(FILE *fichier_encode, Arbre ArbreHuffman) {
    AfficherArbre(ArbreHuffman);

    int erreurRencontre = 0;

    //Initialisation de la variable BFILE* nous permettant de lire les bits encodés du fichier
    BFILE *lecteurFichier = bstart(fichier_encode, "r");


    //Variable de parcours de l'arbre de Huffman
    Arbre noeudParcours = ArbreHuffman;

    //Lecture des bits du fichier
    //Dès lors que le code est celui d'un caractère est reconnu, on l'affiche à l'écran
    int bitLu = bitread(lecteurFichier);
    while (bitLu != -1 && erreurRencontre == 0)
    {
    	//Si le bit lu est 0, on va dans le fils gauche du noeud actuel
    	if(bitLu == 0)
    	{
    		noeudParcours = FilsGauche(noeudParcours);
    	}
    	else
    	{
    		//Si le bit lu est 1, on va dans le fils droit du noeud actuel
    		if(bitLu == 1)
			{
				noeudParcours = FilsDroit(noeudParcours);
			}
			else
			{
				printf("Erreur avec la valeur du bit lu lors du décodage du fichier.\n");

				erreurRencontre = 1;
			}
    	}

    	//Une fois le changement de noeud effectué, on regarde si l'on est sur une feuille ou non.
		//->Si l'on est sur une feuille, le caractère à afficher à l'écran est l'étiquette de la feuille.
		//  Une fois le caractère à afficher, on repart du sommet de l'arbre pour décoder le prochain.
		//->Sinon, on continue le parcours de l'arbre.

		//Si l'on est sur une feuille
		if(EstVide(FilsGauche(noeudParcours)) && EstVide(FilsDroit(noeudParcours)))
		{
			printf("%c", noeudParcours->etiq);

			noeudParcours = ArbreHuffman;
		}

    	bitLu = bitread(lecteurFichier);
    }

    //Si la lecture est terminée alors que l'on est pas au sommet de l'arbre, c'est une erreur.
    if(erreurRencontre == 0 && noeudParcours != ArbreHuffman)
    {
    	printf("Erreur, la lecture est terminée alors que l'on est pas au sommet de l'arbre.\n");
    }
    else
    {
    	printf("\n\n\nDécodage du fichier terminé\n\n.");
    }

}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
    	printf("Erreur avec le nombre de paramètres passés lors de l'appel de cette application.\n");
    }
    else
    {
    	FILE *fichier_encode = fopen(argv[1], "r");

    	//On contrôle que le fichier passé en entrée se soit bien ouvert et ne soit pas vide.
		if(fichier_encode == NULL)
		{
			printf("Erreur, le fichier passé en entrée n'a pas pu être ouvert.\n");
		}
		else
		{
			char premierCaractereFichierLu = fgetc(fichier_encode);

			if(premierCaractereFichierLu == EOF)
			{
				printf("Erreur, le fichier passé en entrée est vide.\n");
			}
			else
			{
				ungetc(premierCaractereFichierLu, fichier_encode);

				//Lecture de l'arbre écrit au début du fichier
				Arbre ArbreHuffman = LireArbre(fichier_encode);

				//Décodage du texte encodé dans le fichier
				Decoder(fichier_encode, ArbreHuffman);

				fclose(fichier_encode);
			}
		}
    }

    return 0;
}
