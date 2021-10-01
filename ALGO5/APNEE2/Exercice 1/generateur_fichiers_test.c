#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

#include "PileTypeParentheses.h"


//Fonctions permettant de générer aléatoirement un nom de fichier
//Source: https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c
static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    if (size) {
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
    }
    return str;
}

//Cette fonction écrit un fichier au nom aléatoire (en prennant garde à le pas écraser un fichier existant) à l'adresse (dossier) indiquée dans adresseFichier et dont le contenu est une suite de caractères contenant des parenthèses/accolades/crochets.
//Le second paramètre qu'elle prend (typeFichierGenere) indique la caractéristique de la chaine de caractères écrite dans le fichier. Il peut prendre 3 valeurs:
//0, si l'on veut une chaine de caractères dont le parenthésage est correct.
//1, si l'on veut une chaine de caractères dont le parenthésage n'est pas correct car le fichier contient trop de parenthèses ouvrantes.
//2, si l'on veut une chaine de caractères dont le parenthésage n'est pas correct car le fichier contient trop de parenthèses fermantes.
//Le troisième paramètre est la taille minimale de la chaine qui sera écrite dans le fichier.
//Le quatrième et dernier paramètre est le niveau d'imbrication minimum du parenthésage du fichier lorsqu'il est parenthésé correctement (dans les autres cas, cette variable n'est pas utilisée car elle ne nous intéresse pas).
void EcrireFichierGenereNomAleatoireDansAdresse(char* adresseFichier, int typeFichierGenere, int nbCaracteresMinimumChaineEcrite, int niveauImbricationMinimumParenthesesFichiersBienParentheses)
{
    //On controle si le type est correct
    if(typeFichierGenere == 0 || typeFichierGenere == 1 || typeFichierGenere == 2)
    {
        //On commence par générer le nom du fichier (de 9 caractères) sans en écraser un existant
        int nomTrouve = 0;
        FILE *fichierTestNom;
        char* nomFichier;
        
        int tailleChaineAdresseFichier = strlen(adresseFichier);
        nomFichier = malloc ((tailleChaineAdresseFichier + 13 + 1) * sizeof(char));
        //On n'oublie pas de copier dans la chaine de caractère l'adresse vers le dossier dans lequel sera rangé le fichier
        strcpy(nomFichier, adresseFichier);

        //On écrit déjà l'extension du fichier car elle est fixe (9 caractères + extension .txt = 13 caractères)
        nomFichier[tailleChaineAdresseFichier + 9] = '.';
        nomFichier[tailleChaineAdresseFichier + 10] = 't';
        nomFichier[tailleChaineAdresseFichier + 11] = 'x';
        nomFichier[tailleChaineAdresseFichier + 12] = 't';
        nomFichier[tailleChaineAdresseFichier + 13] = '\0';

        //Recherche d'un nom de fichier qui n'existe pas déjà
        while (nomTrouve == 0)
        {
            //Génération aléatoire du nom du fichier (9 caractères)
            rand_string(nomFichier + tailleChaineAdresseFichier, 9);

            //On contrôle si un fichier du même nom existe déjà
            //Si un fichier du même nom existe déjà, on reste dans la boucle pour continuer à chercher un nom
            if((fichierTestNom = fopen(nomFichier,"r"))!=NULL)
            {
                fclose(fichierTestNom);
            }
            //Si aucun fichier du même nom n'existe
            else
            {
                nomTrouve = 1;
            }
        }
        
        //Création du fichier
        FILE *fichierEcrit = fopen(nomFichier, "w");

        //Destruction de la chaine contenant le nom du fichier
        free(nomFichier);


        //Ecriture dans le fichier
        //Caractères disponibles à l'écriture (à tirer au hasard)
        const char lettresEtChiffres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        const char parenthesesOuvrantes[] = "({[";
        const char parenthesesFermantes[] = ")}]";

        
        //Pile des parenthèses ouvertes placées que l'on a pas encore fermé. Utile dans le cas où on veut générer un fichier correct
        //On la dépile dès lors qu'une parenthèse fermante est placée à la suite d'une parenthèse ouvrante du même type.
        PileTypeParentheses* pilesTypeParenthesesOuvertesPlacees = NouvellePileTypeParentheses();
        int nbParenthesesOuvrantesPlacees = 0, nbParenthesesFermantesPlacees = 0;
        
        //On tire aléatoirement le nombre de caractères qui seront marqués dans le fichier (en fonction du nombre de caractères minimum saisi par l'utilisateur)
        int nbCaracteres = (rand()%50000) + nbCaracteresMinimumChaineEcrite;
        
        int i = 0, nbCaracteresRestants;

        //Paramètre seulement utilisé pour la génération de fichiers corrects
        //Cette variable contient le niveau maximal d'imbrication des parenthèses du fichier (à comparer avec niveauImbricationMinimumParenthesesFichiersBienParentheses pour générer le fichier correctement)
        int niveauDImbricationMaxFichierEcrit = 0;

        //Paramètres seulement utilisés pour la génération de fichiers incorrects
        int parentheseFermanteDoitEtrePlacee, parentheseOuvranteNePeutEtrePlacee;
        
        int nbAleatoire;
        
        while (i < nbCaracteres)
        {
            nbCaracteresRestants = nbCaracteres - i;
            
            //En fonction du type de fichier à générer demandé, on va chercher le prochain caractère à écrire d'une manière différente
            //Cas dans lequel on veut une chaine de caractères dont le parenthésage est correct.
            if(typeFichierGenere == 0)
            {
                //Pour que le fichier soit généré correctement:
                //-On regarde si l'on doit fermer la/les parenthèses ouverte(s) à cause du fait que l'on arriverait en fin de fichier
                //-On regarde si le niveau minimal de parenthèses ouvertes est atteint. Si il ne l'est pas on regarde le nombre de caractères
                // restants dans le fichier et on décide de si il faut obligatoirement ouvrir une parenthèse pour atteindre ce niveau car
                // il resterait tout juste la place nécessaire à sa fermeture après l'avoir placée.
                int pileNonVide;
                
                TYPES_PARENTHESES typeParentheseSommetPile = NON_DEFINI;
                int profondeurSommet;

                pileNonVide = InfosSommetPile(pilesTypeParenthesesOuvertesPlacees, &typeParentheseSommetPile, &profondeurSommet);

                if(pileNonVide != 1)
                {
                    profondeurSommet = 0;
                }
            
                //Si il ne faut pas obligatoirement placer de parenthèse fermante
                if(nbCaracteresRestants > profondeurSommet)
                {
                    //Si il ne reste pas assez de caractères pouvant être placés dans le fichier pour placer une parenthèse et la fermer au tour suivant
                    if(nbCaracteresRestants == (profondeurSommet + 1))
                    {
                        //On tire au hasard un caractère qui peut soit être une parenthèse fermante, soit une lettre ou un chiffre et on l'écrit dans le fichier
                        //On commence par tirer au hasard le type du caractère écrit.
                        //Si nbAleatoire = 0: Lettre ou chiffre
                        //Si nbAleatoire = 1: Parenthèse fermante

                        nbAleatoire = rand()%2;

                        if(nbAleatoire == 0)
                        {
                            //On tire au hasard un caractère autre qu'une parenthèse et on l'écrit dans le fichier
                            nbAleatoire = rand()%62;
                            fprintf(fichierEcrit, "%c", lettresEtChiffres[nbAleatoire]);
                        }
                        //if(nbAleatoire == 1)
                        else
                        {
                            //On choisit, en fonction de la fernière parenthèse ouverte, la parenthèse fermante à écrire dans le fichier
                            char caractereEcrit;
                            Depiler(pilesTypeParenthesesOuvertesPlacees, &typeParentheseSommetPile, &profondeurSommet);
                            
                            if(typeParentheseSommetPile == PARENTHESE)
                            {
                                caractereEcrit = ')';
                            }
                            else
                            {
                                if(typeParentheseSommetPile == ACCOLADE)
                                {
                                    caractereEcrit = '}';
                                }
                                //if(typeParentheseSommetPile == CROCHET)
                                else
                                {
                                    caractereEcrit = ']';
                                }
                            }

                            fprintf(fichierEcrit, "%c", caractereEcrit);
                        }
                    }
                    //Sinon, on regarde si il faut absolument ouvrir une paranthèse pour atteindre le niveau d'imbrication minimum
                    else
                    {
                        //Si il faut absolument ouvrir une paranthèse pour atteindre le niveau d'imbrication minimum
                        if((niveauDImbricationMaxFichierEcrit < niveauImbricationMinimumParenthesesFichiersBienParentheses)
                            && (((niveauImbricationMinimumParenthesesFichiersBienParentheses - profondeurSommet) * 2) == nbCaracteresRestants))
                        {
                            //On place une parenthèse au hasard
                            //On tire au hasard une parenthèse ouvrante et on l'écrit dans le fichier
                            nbAleatoire = rand()%3;
                            char caractereEcrit = parenthesesOuvrantes[nbAleatoire];
                            fprintf(fichierEcrit, "%c", caractereEcrit);

                            //On empile la parenthèse ouvrante écrite dans la pile
                            if(caractereEcrit == '(')
                            {
                                Empile(pilesTypeParenthesesOuvertesPlacees, PARENTHESE);
                            }
                            else
                            {
                                if(caractereEcrit == '{')
                                {
                                    Empile(pilesTypeParenthesesOuvertesPlacees, ACCOLADE);
                                }
                                //if(caractereEcrit == '[')
                                else
                                {
                                    Empile(pilesTypeParenthesesOuvertesPlacees, CROCHET);
                                }
                            }

                            //On met à jour la variable contenant le niveau d'imbrication maximal du fichier écrit si le niveau
                            //d'imbrication actuel est supérieur à sa valeur
                            if(niveauDImbricationMaxFichierEcrit < (profondeurSommet + 1))
                            {
                                niveauDImbricationMaxFichierEcrit = profondeurSommet + 1;
                            }
                        }
                        //Sinon, on peut placer n'importe quel caractère
                        else
                        {
                            //On commence par tirer au hasard le type du caractère écrit.
                            //Si nbAleatoire = 0: Lettre ou chiffre
                            //Si nbAleatoire = 1: Parenthèse ouvrante
                            //Si nbAleatoire = 2: Parenthèse fermante
                            //On laisse le choix ou non de la parenthèse fermante en fonction de si il est possible ou non de la placer

                            //Cas dans lequel on peut placer une parenthèse fermante
                            if(profondeurSommet > 0)
                            {
                                nbAleatoire = rand()%3;
                            }
                            //Cas dans lequel on ne peut pas
                            else
                            {
                                nbAleatoire = rand()%2;
                            }

                            if(nbAleatoire == 0)
                            {
                                //On tire au hasard un caractère autre qu'une parenthèse et on l'écrit dans le fichier
                                nbAleatoire = rand()%62;
                                fprintf(fichierEcrit, "%c", lettresEtChiffres[nbAleatoire]);
                            }
                            else
                            {
                                if(nbAleatoire == 1)
                                {
                                    //On tire au hasard une parenthèse ouvrante et on l'écrit dans le fichier
                                    nbAleatoire = rand()%3;
                                    char caractereEcrit = parenthesesOuvrantes[nbAleatoire];
                                    fprintf(fichierEcrit, "%c", caractereEcrit);

                                    //On empile la parenthèse ouvrante écrite dans la pile
                                    if(caractereEcrit == '(')
                                    {
                                        Empile(pilesTypeParenthesesOuvertesPlacees, PARENTHESE);
                                    }
                                    else
                                    {
                                        if(caractereEcrit == '{')
                                        {
                                            Empile(pilesTypeParenthesesOuvertesPlacees, ACCOLADE);
                                        }
                                        //if(caractereEcrit == '[')
                                        else
                                        {
                                            Empile(pilesTypeParenthesesOuvertesPlacees, CROCHET);
                                        }
                                    }

                                    //On met à jour la variable contenant le niveau d'imbrication maximal du fichier écrit si le niveau
                                    //d'imbrication actuel est supérieur à sa valeur
                                    if(niveauDImbricationMaxFichierEcrit < (profondeurSommet + 1))
                                    {
                                        niveauDImbricationMaxFichierEcrit = profondeurSommet + 1;
                                    }
                                }
                                //if(nbAleatoire == 2)
                                else
                                {
                                    //On choisit, en fonction de la fernière parenthèse ouverte, la parenthèse fermante à écrire dans le fichier
                                    char caractereEcrit;
                                    Depiler(pilesTypeParenthesesOuvertesPlacees, &typeParentheseSommetPile, &profondeurSommet);
                                    
                                    if(typeParentheseSommetPile == PARENTHESE)
                                    {
                                        caractereEcrit = ')';
                                    }
                                    else
                                    {
                                        if(typeParentheseSommetPile == ACCOLADE)
                                        {
                                            caractereEcrit = '}';
                                        }
                                        //if(typeParentheseSommetPile == CROCHET)
                                        else
                                        {
                                            caractereEcrit = ']';
                                        }
                                    }

                                    fprintf(fichierEcrit, "%c", caractereEcrit);
                                }
                            }
                        }
                    }
                }
                //Sinon, soit il faut obligatoirement placer une parenthèse fermante de bon type, soit le fichier a mal été généré
                else
                {
                    if(nbCaracteresRestants == profondeurSommet)
                    {
                        char caractereEcrit;
                        Depiler(pilesTypeParenthesesOuvertesPlacees, &typeParentheseSommetPile, &profondeurSommet);
                        
                        if(typeParentheseSommetPile == PARENTHESE)
                        {
                            caractereEcrit = ')';
                        }
                        else
                        {
                            if(typeParentheseSommetPile == ACCOLADE)
                            {
                                caractereEcrit = '}';
                            }
                            //if(typeParentheseSommetPile == CROCHET)
                            else
                            {
                                caractereEcrit = ']';
                            }
                        }

                        fprintf(fichierEcrit, "%c", caractereEcrit);
                    }
                    else
                    {
                        printf("Le fichier a mal été généré.\n");
                        return;
                    }
                }
            }
            else
            {
                //Cas dans lequel on veut une chaine de caractères dont le parenthésage n'est pas correct car le fichier contient trop de parenthèses ouvrantes.
                if(typeFichierGenere == 1)
                {
                    
                }
                //Cas dans lequel on veut une chaine de caractères dont le parenthésage n'est pas correct car le fichier contient trop de parenthèses fermantes.
                //if(typeFichierGenere == 2)
                else
                {
                    
                }
            }
            
            i = i + 1;
        }

        //Destruction de la pile
        DetruirePileTypeParentheses(pilesTypeParenthesesOuvertesPlacees);
        
        //Fermeture du fichier
        fclose(fichierEcrit);
    }
    else
    {
        printf("Le type de fichier à générer indiqué n'est pas correct.\n");
    }
}


int main (int argc, char *argv[])
{
    // Les six paramètres du programme sont le nombre de fichiers correctement parenthésés, l'adresse dans lesquels seront rangés ces fichiers; non-correctement car trop de parenthèses ouvrantes, l'adresse dans lesquels seront rangés ces fichiers; non-correctement car trop de parenthèses fermantes parenthéses à générer, l'adresse dans lesquels seront rangés ces fichiers.
    if(argc == 7)
    {
        //L'application affiche les erreurs de débug si debug = 1;
        int debug = 1;

        // Initialisation de la fonction random
        srand(time(NULL));


        int nbFichiersBienParentheses, nbFichiersTropParenthesesOuvrantes, nbFichiersTropParenthesesFermantes;

        sscanf(argv[1], " %d ", &nbFichiersBienParentheses);
        char* adresseFichiersBienParentheses = argv[2];
        
        sscanf(argv[3], " %d ", &nbFichiersTropParenthesesOuvrantes);
        char* adresseFichiersTropParenthesesOuvrantes = argv[4];
        
        sscanf(argv[5], " %d ", &nbFichiersTropParenthesesFermantes);
        char* adresseFichiersTropParenthesesFermantes = argv[6];


        //On commence par contrôler si les adresses de dossiers indiquées sont accessibles
        DIR* dossier;
        //Dossier dans lequel seront rangés les fichiers corrects
        dossier = opendir(adresseFichiersBienParentheses);
        //Le dossier est accessible
        if(dossier)
        {
            closedir(dossier);
        }
        else
        {
            //Le dossier n'est pas accessible car il n'existe pas
            if (errno == ENOENT)
            {
                //printf("Erreur, le dossier dans lequel il faut ranger les fichiers bien parenthésés n'existe pas.\n");
                printf("%s\n", adresseFichiersBienParentheses);
            }
            //Le dossier n'est pas accessible pour une autre raison
            else
            {
                printf("Erreur, le dossier dans lequel il faut ranger les fichiers bien parenthésés est inaccessible pour une raison autre qu'il n'existe pas.\n");
            }
        }
        
        //Dossier dans lequel seront ranger les fichiers incorrects car trop de parenthèses ouvrantes
        dossier = opendir(adresseFichiersTropParenthesesOuvrantes);
        //Le dossier est accessible
        if(dossier)
        {
            closedir(dossier);
        }
        else
        {
            //Le dossier n'est pas accessible car il n'existe pas
            if (errno == ENOENT)
            {
                printf("Erreur, le dossier dans lequel il faut ranger les fichiers mal parenthésés (trop de parenthèses ouvrantes) n'existe pas.\n");
            }
            //Le dossier n'est pas accessible pour une autre raison
            else
            {
                printf("Erreur, le dossier dans lequel il faut ranger les fichiers bien parenthésés est inaccessible pour une raison autre qu'il n'existe pas.\n");
            }
        }
        
        //Dossier dans lequel seront ranger les fichiers incorrects car trop de parenthèses fermantes
        dossier = opendir(adresseFichiersTropParenthesesFermantes);
        //Le dossier est accessible
        if(dossier)
        {
            closedir(dossier);
        }
        else
        {
            //Le dossier n'est pas accessible car il n'existe pas
            if (errno == ENOENT)
            {
                printf("Erreur, le dossier dans lequel il faut ranger les fichiers mal parenthésés (trop de parenthèses fermantes) n'existe pas.\n");
            }
            //Le dossier n'est pas accessible pour une autre raison
            else
            {
                printf("Erreur, le dossier dans lequel il faut ranger les fichiers bien parenthésés est inaccessible pour une raison autre qu'il n'existe pas.\n");
            }
        }


        //On demande ensuite à l'utilisateur le nombre de caractères minimum que feront les chaines de caractères des fichiers générés
        //->Entre 0 et 50 000
        int tailleMinimumNbCaracteres;
        printf("\nSaisissez le nombre de caractères minimum que feront les chaines de caractères des fichiers générés (entre 0 et 50000).\nSi la taille minimum ne vous importe pas, saisissez 0: ") ;
        scanf("%d", &tailleMinimumNbCaracteres);
        while(tailleMinimumNbCaracteres < 0 || tailleMinimumNbCaracteres > 50000)
        {
            printf("\nErreur avec le paramètre saisi.\nSaisissez le nombre de caractères minimum que feront les chaines de caractères des fichiers générés (entre 0 et 50000).\nSi la taille minimum ne vous importe pas, saisissez 0: ") ;
            scanf("%d", &tailleMinimumNbCaracteres);
        }

        //On demande ensuite à l'utilisateur le niveau d'imbrication minimum de parenthèses que devra avoir le fichier généré (pour les fichiers générés correctement)
        //Il faut que ce niveau d'imbrication soit <= (tailleMinimumNbCaracteres / 2) sinon la chaine de caractères ne peut être générée (pas assez de caractères pour ouvrir puis fermer les parenthèses correctement)
        //->Entre 0 et (tailleMinimumNbCaracteres/2), si l'utilisateur ne saisi rien, sa taille minimum est de 0 caractères
        int niveauImbricationMinimumParenthesesFichiersBienParentheses;
        printf("\nSaisissez le niveau d'imbrication minimum de parenthèses que devra avoir le fichier généré (pour les fichiers générés correctement).\n") ;
        printf("Il faut que ce niveau d'imbrication soit <= (tailleMinimumNbCaracteres / 2) sinon la chaine de caractères ne peut être générée (pas assez de caractères pour ouvrir puis fermer les parenthèses correctement).\nSi le niveau d'imbrication minimum ne vous importe pas, saisissez 0: ") ;
        scanf("%d", &niveauImbricationMinimumParenthesesFichiersBienParentheses);
        while(niveauImbricationMinimumParenthesesFichiersBienParentheses < 0 || niveauImbricationMinimumParenthesesFichiersBienParentheses > ((double)tailleMinimumNbCaracteres / 2))
        {
            printf("\nErreur avec le paramètre saisi.\nSaisissez le niveau d'imbrication minimum de parenthèses que devra avoir le fichier généré (pour les fichiers générés correctement).\n") ;
            printf("Il faut que ce niveau d'imbrication soit <= (tailleMinimumNbCaracteres / 2) sinon la chaine de caractères ne peut être générée (pas assez de caractères pour ouvrir puis fermer les parenthèses correctement).\nSi le niveau d'imbrication minimum ne vous importe pas, saisissez 0: ") ;
            scanf("%d", &niveauImbricationMinimumParenthesesFichiersBienParentheses);
        }

        
        int i;
        
        //Génération des fichiers corrects
        i = 0;
            
        while(i < nbFichiersBienParentheses)
        {
            //On appelle la fonction EcrireFichierGenereNomAleatoireDansAdresse
            //Elle écrit un fichier au nom aléatoire (en prennant garde à le pas écraser un fichier existant) à l'adresse indiquée dans adresseFichier et dont le contenu est une suite de caractères contenant des parenthèses/accolades/crochets.
            //Le second paramètre qu'elle prend (typeFichierGenere) indique la caractéristique de la chaine de caractères écrite dans le fichier. Il peut prendre 3 valeurs:
            //0, si l'on veut une chaine de caractères dont le parenthésage est correct.
            //1, si l'on veut une chaine de caractères dont le parenthésage n'est pas correct car le fichier contient trop de parenthèses ouvrantes.
            //2, si l'on veut une chaine de caractères dont le parenthésage n'est pas correct car le fichier contient trop de parenthèses fermantes.
            //Le troisième paramètre est la taille minimale de la chaine qui sera écrite dans le fichier.
            //Le quatrième et dernier paramètre est le niveau d'imbrication minimum du parenthésage du fichier.
            EcrireFichierGenereNomAleatoireDansAdresse(adresseFichiersBienParentheses, 0, tailleMinimumNbCaracteres, niveauImbricationMinimumParenthesesFichiersBienParentheses);

            i = i + 1;
        }
        
        
        //Génération des fichiers incorrects de type 1
        i = 0;
            
        while(i < nbFichiersTropParenthesesOuvrantes)
        {
            //Dans cet appel de fonction, le niveau d'imbrication minimum du parenthésage du fichier est 0 zéro car il ne nous intéresse pas lors de la génération de fichiers incorrects.
            EcrireFichierGenereNomAleatoireDansAdresse(adresseFichiersTropParenthesesOuvrantes, 1, tailleMinimumNbCaracteres, 1);
            
            i = i + 1;
        }
        
        
        //Génération des fichiers incorrects de type 2
        i = 0;
            
        while(i < nbFichiersTropParenthesesFermantes)
        {
            //Dans cet appel de fonction, le niveau d'imbrication minimum du parenthésage du fichier est 0 zéro car il ne nous intéresse pas lors de la génération de fichiers incorrects.
            EcrireFichierGenereNomAleatoireDansAdresse(adresseFichiersTropParenthesesFermantes, 2, tailleMinimumNbCaracteres, 2);
            
            i = i + 1;
        }
    }
    else
    {
        printf("Le nombre d'arguments est incorrect, il faut spécifier le nombre de fichiers correctement parenthésés, l'adresse dans lesquels seront rangés ces fichiers; non-correctement car trop de parenthèses ouvrantes, l'adresse dans lesquels seront rangés ces fichiers; non-correctement car trop de parenthèses fermantes parenthéses à générer, l'adresse dans lesquels seront rangés ces fichiers..\n");
    }

    return 0 ;
}