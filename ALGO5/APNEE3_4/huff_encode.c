#include "arbrebin.h"
#include "bfile.h"
#include "fap.h"
#include "huffman_code.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int tab[256];
} TableOcc_t;

struct code_char HuffmanCode[256];

char **T;

//L'attribut tab de TableOcc est remis à zéro en début de fonction
void ConstruireTableOcc(FILE *fichier, TableOcc_t *TableOcc) {
    //Caractère lu dans le fichier
    int c;

    //Remise à zéro de l'attribut tab de TableOcc
    int i = 0;
    while (i < 256)
    {
        TableOcc->tab[i] = 0;

        i = i + 1;
    }


    //On récupère le premier caractère du fichier
    c = fgetc(fichier);

    //On compte les occurences de chaque caractères
    while (c != EOF) {
        TableOcc->tab[c] = TableOcc->tab[c] + 1;

        //On récupère le prochain caractère
        c = fgetc(fichier);
    };


    for (i = 0; i < 256; i++) {
        if (TableOcc->tab[i] != 0)
            printf("Occurences du caractere %c (code %d) : %d\n", i, i,
                   TableOcc->tab[i]);
    }
}

//Cette fonction initialise la fap qui sera utilisée par l'algorithme de Huffman en y placant dedans les caractères et le nombre de fois qu'ils apparaissent dans le texte.
fap InitHuffman(TableOcc_t *TableOcc) {
    //File à priorité retournée
    fap fileCreee = creer_fap_vide();

    int i = 0;

    //On n'insère dans la file que les caractères apparaissant au moins une fois dans le texte.
    while (i < 256)
    {
        if(TableOcc->tab[i] > 0)
        {
            fileCreee = inserer(fileCreee, NouveauNoeud(ArbreVide(), i, ArbreVide()), TableOcc->tab[i]);
        }

        i = i + 1;
    }

    return fileCreee;
}


Arbre ConstruireArbre(fap file) {
    //Arbre de Huffman retourné
    Arbre arbreRetourne = ArbreVide();


    //Variables utiles à la construction de l'arbre
    Arbre arbre1File, arbre2File;
    int prioriteArbre1File, prioriteArbre2File;


    //Tant que la fap contient plus d'un élément, la construction de l'arbre n'est pas terminée
    while(!est_fap_vide(file))
    {
        //On extrait les deux éléments les plus prioritaires de la file à priorité
        file = extraire(file, &arbre1File, &prioriteArbre1File);

        //Si il n'y avait qu'un seul élément dans la file, la construction de l'arbre est terminée.
        //Sinon, on continue l'exécution de l'algorithme
        if(!est_fap_vide(file))
        {
            file = extraire(file, &arbre2File, &prioriteArbre2File);


            //On fusionne les deux arbres et on place cet arbre dans la file
            //Le noeud dont les fils sont les deux arbres extraits de la file a comme élément ' '
            //->Cet élément ne signifie rien de particulier, les caractères encodés par l'algorithme de Huffman étant positionnés sur les feuilles

            //On place arbitrairement le premier arbre extrait de la file à gauche et le second à droite.
            file = inserer(file, NouveauNoeud(arbre1File, '_', arbre2File), prioriteArbre1File + prioriteArbre2File);
        }
        //Cas dans lequel l'arbre de Huffman est construit
        else
        {
            arbreRetourne = arbre1File;
        }
    }

    return arbreRetourne;
}




void ConstruireCodeRec(Arbre huff, char* w) {
  if(!EstVide(FilsGauche(huff)) && !EstVide(FilsDroit(huff))) {
    w++;
    *w = '0';
    ConstruireCodeRec(FilsGauche(huff),w);
    *w = '1';
    ConstruireCodeRec(FilsDroit(huff),w);
  } else {
    int i = 0;
    while (w[i] == '0' || w[i] == '1') {
      T[huff->etiq][i] = w[i];
      printf("%s\n",w);
      i++;
    }
  }
}

void ConstruireCode(Arbre huff) {
    T = malloc(sizeof(char*)*256);
    for(int i = 0; i < 256; i++){
      T[i] = malloc(sizeof(char)*256);
    }
    char *w = malloc(sizeof(char)*256);
    ConstruireCodeRec(huff, w);
    //printf("Programme non realise (ConstruireCode)\n");
}

void Encoder(FILE *fic_in, FILE *fic_out, Arbre ArbreHuffman) {
    /* A COMPLETER */
    printf("Programme non realise (Encoder)\n");
}

int main(int argc, char *argv[]) {

    TableOcc_t TableOcc;
    FILE *fichier;
    FILE *fichier_encode;

    fichier = fopen(argv[1], "r");
    /* Construire la table d'occurences */
    ConstruireTableOcc(fichier, &TableOcc);
    fclose(fichier);

    /* Initialiser la FAP */
    fap file = InitHuffman(&TableOcc);

    /* Construire l'arbre d'Huffman */
    Arbre ArbreHuffman = ConstruireArbre(file);

        AfficherArbre(ArbreHuffman);

    /* Construire la table de codage */
    ConstruireCode(ArbreHuffman);

    /* Encodage */
    fichier = fopen(argv[1], "r");
    fichier_encode = fopen(argv[2], "w");

    Encoder(fichier, fichier_encode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier);

    return 0;
}
