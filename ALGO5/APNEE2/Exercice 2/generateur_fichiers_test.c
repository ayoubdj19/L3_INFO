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
//1, si l'on veut une chaine de caractères dont le parenthésage n'est pas correct.
//Le troisième paramètre est la taille minimale de la chaine qui sera écrite dans le fichier.
//Le quatrième et dernier paramètre est le niveau d'imbrication minimum du parenthésage du fichier lorsqu'il est parenthésé correctement (dans le cas où l'on génère des fichiers mal parenthésés, cette variable n'est pas utilisée car elle ne nous intéresse pas).
void EcrireFichierGenereNomAleatoireDansAdresse(char* adresseFichier, int typeFichierGenere, int nbCaracteresMinimumChaineEcrite, int niveauImbricationMinimumParenthesesFichiersBienParentheses)
{
    //On controle si le type est correct
    if(typeFichierGenere == 0 || typeFichierGenere == 1)
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
        int pileNonVide;
        TYPES_PARENTHESES typeParentheseSommetPile = NON_DEFINI;
        TYPES_PARENTHESES typeParentheseCaractereEcrit = NON_DEFINI;
        int profondeurSommet;
        int i = 0, nbCaracteresRestants;

        //Variables utiles pour savoir quel caractère doit être écrit dans le fichier
        int nbAleatoire;
        char caractereEcrit;
        
        //On tire aléatoirement le nombre de caractères qui seront marqués dans le fichier (en fonction du nombre de caractères minimum saisi par l'utilisateur)
        int nbCaracteres = (rand()%50000) + nbCaracteresMinimumChaineEcrite;

        //Variable seulement utilisée pour la génération de fichiers corrects
        //Cette variable contient le niveau maximal d'imbrication des parenthèses du fichier (à comparer avec niveauImbricationMinimumParenthesesFichiersBienParentheses pour générer le fichier correctement)
        int niveauDImbricationMaxFichierEcrit = 0;

        //Variable seulement utilisée pour la génération de fichiers incorrects
        int fichierMalGenere = 0;
        
        
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
                            caractereEcrit = parenthesesOuvrantes[nbAleatoire];
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
                                    caractereEcrit = parenthesesOuvrantes[nbAleatoire];
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
            //Cas dans lequel on veut une chaine de caractères dont le parenthésage n'est pas correct.
            //if(typeFichierGenere == 1)
            else
            {
                //Pour que le fichier soit généré de manière incorrecte à coup sûr, il faut que l'une de ces 2 conditions soit respectée:
                //-Une parenthèse a été ouverte mais n'a jamais été fermée
                //-Une parenthèse fermante est placée alors que son équivalent ouvrant n'était pas la dernière parenthèse écrite dans le fichier

                //On va donc utiliser une variable booléenne indiquant si au cours de la génération du fichier, on est dans l'une ces situations
                //Si l'on arrive à la fin du fichier et qu'aucune de ces condition n'est respectée, on vient faire exprès de placer une
                //parenthèse fermante qui n'a pas été ouverte/ouvrir une parenthèse sans la fermer (car on est à la fin du fichier) afin que le fichier ne soit plus parenthésé correctement
                //->Le choix de procéder d'une manière ou d'une autre est pris aléatoirement
                //La variable booléene est: fichierMalGenere

                //On va utiliser une pile nous permettant de savoir quelles parenthèses ont été ouvertes et ne sont pas encore fermées
                //pour contrôler ces conditions
                //->Dès lors que le fichier a une erreur, on arrête de gérer la pile, la variable booléene correspondant au fait
                // qu'il y a une erreur prend comme valeur 1, et le fichier est générée de manière totalement aléatoire jusqu'à la fin.


                //Dans cette génération, le type de caractère écrit est choisi de manière aléatoire durant toute la génération du fichier (sauf à la fin)


                //On, regarde si le caractère que l'on doit placer dans le fichier peut être choisi de manière totalement aléatoire
                //(si on n'est pas en train de choisir le dernier caractère écrit dans le fichier/on est déjà sûr que le fichier généré a une erreur)
                if(nbCaracteresRestants > 1 || fichierMalGenere == 1)
                {
                    //On commence par tirer au hasard le type du caractère écrit.
                    //Si nbAleatoire = 0: Lettre ou chiffre
                    //Si nbAleatoire = 1: Parenthèse ouvrante
                    //Si nbAleatoire = 2: Parenthèse fermante
                    //On laisse le choix ou non de la parenthèse fermante en fonction de si il est possible ou non de la placer

                    nbAleatoire = rand()%3;

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
                            caractereEcrit = parenthesesOuvrantes[nbAleatoire];
                            fprintf(fichierEcrit, "%c", caractereEcrit);

                            //Car le fichier n'est pas encore incorrect à coup sûr, on empile la parenthèse ouvrante écrite dans la pile
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
                            //On tire au hasard une parenthèse fermante et on l'écrit.
                            //Si le parenthésage est déjà incorrect, on ne fait pas plus
                            //Par contre dans le cas inverse, si la parenthèse tirée est cohérente par rapport à la dernière
                            //parenthèse ouverte (si il y en a une), on dépile la pile et le parenthésage reste correct.
                            //Si elle n'est pas correcte (pas de parenthèse ouverte/parenthèse ouverte d'un type différent),
                            //on est sûr que le parenthésage est incorrect et on passe fichierMalGenere à 1.
                            nbAleatoire = rand()%3;
                            caractereEcrit = parenthesesFermantes[nbAleatoire];
                            fprintf(fichierEcrit, "%c", caractereEcrit);


                            //On regarde si le parenthésage était déjà incorrect
                            //Si il ne l'était pas
                            if(fichierMalGenere == 0)
                            {
                                pileNonVide = InfosSommetPile(pilesTypeParenthesesOuvertesPlacees, &typeParentheseSommetPile, &profondeurSommet);

                                //Si la pile est vide, le parenthésage est alors incorrect
                                if(pileNonVide != 1)
                                {
                                    fichierMalGenere = 1;
                                }
                                else
                                {
                                    //Si le type de parenthèse du sommet de la pile est différent de celui de la parenthèse fermante
                                    //écrite, le parenthésage est alors incorrect
                                    typeParentheseCaractereEcrit = getTypeParentheseCaractere(caractereEcrit);

                                    if(typeParentheseCaractereEcrit != ERREUR)
                                    {
                                        //Parenthésage incorrect
                                        if(typeParentheseCaractereEcrit != typeParentheseSommetPile)
                                        {
                                            fichierMalGenere = 1;
                                        }
                                        //Sinon, on dépile et le parenthésage reste correct
                                        {
                                            Depiler(pilesTypeParenthesesOuvertesPlacees, &typeParentheseSommetPile, &profondeurSommet);
                                        }
                                    }
                                    else
                                    {
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
                //Si le caractère que l'on doit placer dans le fichier ne peut être choisi de manière totalement aléatoire sous peine de générer un fichier correct
                else
                {
                    //On commence par choisir au hasard la manière dont on fera en sorte que le parenthésage soit incorrect/
                    //Si nbAleatoire = 0: En plaçant une parenthèse ouvrante (si l'on est dans ce else, cela signifie que l'on est forcément en train d'écrire le dernier caractère du fichier et la parenthèse placée ne pourra pas être fermée) 
                    //Si nbAleatoire = 1: En plaçant une parenthèse fermante
                    //                    ->Si aucune parenthèse n'est ouverte, la parenthèse placée est tirée totalement au hasard
                    //                    ->Sinon, la parenthèse placée est tirée au hasard entre les deux types de parenthèses qui ne sont pas celui de la parenthèse ouverte
                    //On laisse le choix ou non de la parenthèse fermante en fonction de si il est possible ou non de la placer

                    nbAleatoire = rand()%2;

                    if(nbAleatoire == 0)
                    {
                        //On tire au hasard une parenthèse ouvrante et on l'écrit dans le fichier
                        nbAleatoire = rand()%3;
                        caractereEcrit = parenthesesOuvrantes[nbAleatoire];
                        fprintf(fichierEcrit, "%c", caractereEcrit);
                    }
                    //if(nbAleatoire == 1)
                    else
                    {
                        //On tire au hasard une parenthèse fermante (de type différent de la dernière ouvrante pas encore fermée écrite dans le fichier si il y en a une)
                        
                        //On commence par consulter les informations de la pile
                        pileNonVide = InfosSommetPile(pilesTypeParenthesesOuvertesPlacees, &typeParentheseSommetPile, &profondeurSommet);

                        //Si la pile est vide, aucune parenthèse ouvrante pas encore fermée n'est écrite dans le fichier et on peut
                        //placer n'importe quelle parenthèse
                        if(pileNonVide != 1)
                        {
                            nbAleatoire = rand()%3;
                            caractereEcrit = parenthesesFermantes[nbAleatoire];
                        }
                        else
                        {
                            //Si la pile n'est pas vide, on écrit une parenthèse d'un type différent de celui du sommet (toujours de manière aléatoire)
                            char parenthesesFermantesTirables[2];

                            if(typeParentheseSommetPile == PARENTHESE)
                            {
                                parenthesesFermantesTirables[0] = '}';
                                parenthesesFermantesTirables[1] = ']';
                            }
                            else
                            {
                                if(typeParentheseSommetPile == ACCOLADE)
                                {
                                    parenthesesFermantesTirables[0] = ')';
                                    parenthesesFermantesTirables[1] = ']';
                                }
                                else
                                {
                                    if(typeParentheseSommetPile == CROCHET)
                                    {
                                        parenthesesFermantesTirables[0] = ')';
                                        parenthesesFermantesTirables[1] = '}';
                                    }
                                    else
                                    {
                                        return;
                                    }
                                }
                            }
                            
                            nbAleatoire = rand()%2;
                            caractereEcrit = parenthesesFermantesTirables[nbAleatoire];
                        }

                        //On écrit la parenthèse fermante dans le fichier
                        fprintf(fichierEcrit, "%c", caractereEcrit);
                    }
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
    // Les sept paramètres du programme sont: l'adresse dans laquelle seront rangés les fichiers bien parenthésés, combien de ces fichiers seront générés, leur nombre de caractères minimum (>= 0), le niveau d'imbrication minimum de parenthèses que devront avoir les fichiers générés;
    // l'adresse dans laquelle seront rangés les fichiers bien parenthésés, combien de ces fichiers seront générés, leur nombre de caractères minimum (> 0 car sinon il est impossible de générer un fichier dont le parenthésage est incorrect).
    
    //Nombre de caractères minimum que feront les chaines de caractères des fichiers générés correctement
    //->Entre 0 et 50 000

    //Nombre de caractères minimum que feront les chaines de caractères des fichiers générés incorrectement
    //->Entre 1 et 50 000 (car un fichier ne peut pas être généré incorrectement si il ne fait pas au minimum 1 caractère):
    //  Les deux cas d'erreurs possibles étant:
    //  -Une parenthèse a été ouverte mais n'a jamais été fermée
    //  -Une parenthèse fermante est placée alors que son équivalent ouvrant n'était pas la dernière parenthèse écrite dans le fichier
    //   =>Il faut dans les deux cas écrire au moins une parenthèse ouvrante/fermante

    if(argc != 8)
    {
        printf("Le nombre d'arguments est incorrect:\n");
        printf("Il faut spécifier l'adresse dans laquelle seront rangés les fichiers bien parenthésés, combien de ces fichiers seront générés, leur nombre de caractères minimum (>= 0), le niveau d'imbrication minimum de parenthèses que devront avoir les fichiers générés;\n");
        printf("l'adresse dans laquelle seront rangés les fichiers bien parenthésés, combien de ces fichiers seront générés, leur nombre de caractères minimum (> 0 car sinon il est impossible de générer un fichier dont le parenthésage est incorrect).\n\n");
    }
    else
    {
        int erreurParametres = 0;

        //Paramètres fichiers bien parenthésés
        char* adresseFichiersBienParentheses = argv[1];
        int nbFichiersBienParentheses;
        sscanf(argv[2], " %d ", &nbFichiersBienParentheses);
        int tailleMinimumNbCaracteresFichiersBienParentheses;
        sscanf(argv[3], " %d ", &tailleMinimumNbCaracteresFichiersBienParentheses);
        int niveauImbricationMinimumParenthesesFichiersBienParentheses;
        sscanf(argv[4], " %d ", &niveauImbricationMinimumParenthesesFichiersBienParentheses);

        //Paramètres fichiers mal parenthésés
        char* adresseFichiersMalParentheses = argv[5];
        int nbFichiersMalParentheses;
        sscanf(argv[6], " %d ", &nbFichiersMalParentheses);
        int tailleMinimumNbCaracteresFichiersMalParentheses;
        sscanf(argv[7], " %d ", &tailleMinimumNbCaracteresFichiersMalParentheses);


        //On commence par contrôler si les adresses de dossiers indiquées sont accessibles
        DIR* dossier;
        //Dossier dans lequel seront rangés les fichiers corrects
        dossier = opendir(adresseFichiersBienParentheses);
        //Le dossier n'est pas accessible
        if(!dossier)
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
            erreurParametres = 1;
        }
        //Le dossier est accessible
        else
        {
            closedir(dossier);

            //Dossier dans lequel seront rangés les fichiers incorrects
            dossier = opendir(adresseFichiersMalParentheses);
            //Le dossier n'est pas accessible
            if(!dossier)
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
                erreurParametres = 1;
            }
            //Le dossier est accessible
            else
            {
                closedir(dossier);

                //On contrôle si le nombre de fichiers à parenthéser est correct (>= 0 et <= 50000)
                if(nbFichiersBienParentheses < 0 || nbFichiersBienParentheses > 50000)
                {
                    printf("Erreur avec le nombre de fichier dont le parenthésage est correct à générer. Ce dernier doit être entre 0 et 50000 (tous deux compris).\n") ;
                    erreurParametres = 1;
                }
                else
                {
                    if(nbFichiersMalParentheses < 0 || nbFichiersMalParentheses > 50000)
                    {
                        printf("Erreur avec le nombre de fichier dont le parenthésage est incorrect à générer. Ce dernier doit être entre 0 et 50000 (tous deux compris).\n") ;
                        erreurParametres = 1;
                    }
                    else
                    {
                        //On contrôle si les tailles des chaines de caractères qui seront écrites dans les fichiers sont correctes
                        if(tailleMinimumNbCaracteresFichiersBienParentheses < 0 || tailleMinimumNbCaracteresFichiersBienParentheses > 50000)
                        {
                            printf("Erreur avec le nombre de caractères minimum que feront les chaines de caractères correctes générées saisi. Ce dernier doit être entre 0 et 50000 (tous deux compris).\n") ;
                            erreurParametres = 1;
                        }
                        else
                        {
                            if(tailleMinimumNbCaracteresFichiersMalParentheses < 1 || tailleMinimumNbCaracteresFichiersMalParentheses > 50000)
                            {
                                printf("Erreur avec le nombre de caractères minimum que feront les chaines de caractères incorrectes générées saisi. Ce dernier doit être entre 0 (non compris) et 50000 (compris).\n") ;
                                erreurParametres = 1;
                            }
                            else
                            {
                                //On contrôle le niveau d'imbrication minimum de parenthèses que devront avoir les fichiers générés correctement
                                //Il faut que ce niveau d'imbrication soit <= (tailleMinimumNbCaracteres / 2) sinon la chaine de caractères ne peut être générée (pas assez de caractères pour ouvrir puis fermer les parenthèses correctement)
                                //->Entre 0 et (tailleMinimumNbCaracteres/2). Si niveauImbricationMinimumParenthesesFichiersBienParentheses = 0, le fichier peut ne générer aucune chaine de caractères.
                                if(niveauImbricationMinimumParenthesesFichiersBienParentheses < 0 || niveauImbricationMinimumParenthesesFichiersBienParentheses > (((double)tailleMinimumNbCaracteresFichiersMalParentheses) / 2))
                                {
                                    printf("\nErreur avec le niveau d'imbrication minimum de parenthèses que devra avoir le fichier généré (pour les fichiers générés correctement) saisi.\n") ;
                                    printf("Il faut que ce niveau d'imbrication soit <= ((taille chaines de caractères correctes générés) / 2) sinon la chaine de caractères ne peut être générée correctement (pas assez de caractères pour ouvrir puis fermer les parenthèses correctement).\n") ;
                                    erreurParametres = 1;
                                }
                            }
                        }
                    }
                }
            }
        }


        //Si il n'y a aucune erreur de paramètres
        if(erreurParametres == 0)
        {
            // Initialisation de la fonction random
            srand(time(NULL));

            
            int i;
            
            //Génération des fichiers corrects
            i = 0;
                
            while(i < nbFichiersBienParentheses)
            {
                //On appelle la fonction EcrireFichierGenereNomAleatoireDansAdresse
                //Elle écrit un fichier au nom aléatoire (en prennant garde à le pas écraser un fichier existant) à l'adresse indiquée dans adresseFichier et dont le contenu est une suite de caractères contenant des parenthèses/accolades/crochets.
                //Le second paramètre qu'elle prend (typeFichierGenere) indique la caractéristique de la chaine de caractères écrite dans le fichier. Il peut prendre 3 valeurs:
                //0, si l'on veut une chaine de caractères dont le parenthésage est correct.
                //1, si l'on veut une chaine de caractères dont le parenthésage n'est pas correct.
                //Le troisième paramètre est la taille minimale de la chaine qui sera écrite dans le fichier.
                //Le quatrième et dernier paramètre est le niveau d'imbrication minimum du parenthésage du fichier.
                EcrireFichierGenereNomAleatoireDansAdresse(adresseFichiersBienParentheses, 0, tailleMinimumNbCaracteresFichiersBienParentheses, niveauImbricationMinimumParenthesesFichiersBienParentheses);

                i = i + 1;
            }
            
            
            //Génération des fichiers incorrects
            i = 0;
            
            while(i < nbFichiersMalParentheses)
            {
                //Dans cet appel de fonction, le niveau d'imbrication minimum du parenthésage du fichier est 0 zéro car il ne nous intéresse pas lors de la génération de fichiers incorrects.
                EcrireFichierGenereNomAleatoireDansAdresse(adresseFichiersMalParentheses, 1, tailleMinimumNbCaracteresFichiersMalParentheses, 0);
                
                i = i + 1;
            }
        }
    }

    return 0 ;
}