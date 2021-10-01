#ifndef __PILE_TYPE_PARENTHESES__
#define __PILE_TYPE_PARENTHESES__

//Types de parenthèses différentes
typedef enum { 
    PARENTHESE,
    ACCOLADE,
    CROCHET,
    NON_DEFINI,
    ERREUR
} TYPES_PARENTHESES;


struct StructElementPileTypeParentheses {
  TYPES_PARENTHESES typeParenthese;

  //Une parenthèse placée sans rien autour est de profondeur 1.
  //Si elle est elle même dans une parenthèse, sa profondeur est de 2, etc...
  int profondeur;
  struct StructElementPileTypeParentheses *suivant;
};
typedef struct StructElementPileTypeParentheses ElementPileTypeParentheses;

//Pile de types de parenthèses (pour garder en mémoire ce qui a été rencontré)
typedef struct {
  ElementPileTypeParentheses *tete;
} PileTypeParentheses;


//Fonctions de construction
PileTypeParentheses* NouvellePileTypeParentheses(void);
ElementPileTypeParentheses* NouvelElementPileTypeParentheses(void);


//Fonctions de destruction
void DetruirePileTypeParentheses(PileTypeParentheses* pileADetruire);
void DetruireElementPileTypeParenthesesEtSaDescendance(ElementPileTypeParentheses* elementADetruire);
void DetruireElementPileTypeParentheses(ElementPileTypeParentheses* elementADetruire);


//Autres fonctions
//Retour dans les pointeurs le type de parenthèse en tête de la pile et le supprime de cette dernière
//Si la pile est vide, la fonction retourne 0, sinon elle retourne 1.
int Depiler(PileTypeParentheses* pileADepiler, TYPES_PARENTHESES* typeParentheseElementDepile, int* profondeurParentheseElementDepile);

//La profondeur est automatiquement calculée en fonction de celle du sommet (zero si il n'y en avait pas)
void Empile(PileTypeParentheses* pileAEmpiler, TYPES_PARENTHESES typeParentheseElementEmpiler);

//Retour 1 si la pile est vide, 0 si elle ne l'est pas
int EstPileVide(PileTypeParentheses* pileAControler);

//Retourne dans les pointeurs passés en paramètre le type et la profondeur de la parenthèse au sommet de la pile (si elle n'est pas vide).
//Si la pile est vide, la fonction retourne 0, sinon elle retourne 1.
int InfosSommetPile(PileTypeParentheses* laPile, TYPES_PARENTHESES* typeParentheseSommet, int* profondeurParentheseSommet);



//Retourne le type de parenthèse d'un caractère passé en paramètre
//Si le caractère n'est pas une parenthèse, le retour est: ERREUR
TYPES_PARENTHESES getTypeParentheseCaractere(char caractere);

#endif