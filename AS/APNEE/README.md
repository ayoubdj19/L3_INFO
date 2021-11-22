# Instructions

Pour pouvoir compiler le projet sur votre ordinateur, il vous faut installer les librairies flex et bison.

- Sur linux (ubuntu) cela se fait par la commande :

  sudo apt-get install flex bison

  (ou équivalent sur d'autres distributions)

- Sur mac, ces librairies sont installées en standard. Si ce n'est pas le cas pour vous, cela peut s'installer via homebrew (cf https://brew.sh/index_fr)

  brew install flex bison

- sur windows, je ne sais pas, à vous de chercher.

# Compte rendu

## Grammaires utilisée pour les différentes étapes

1ère étape : Ecriture de la grammaire du langage.

Grammaire obtenue :
R1 : S -> v = A ;
R2 : A -> v = A
R3 : A -> E
R4 : E -> E O F
R5 : E -> F
R6 : F -> x
R7 : F -> ( A )
R8 : F -> | E |
R9 : O -> +
R10 : O -> -
R11 : O -> *
R12 : O -> /
R13 : O -> ^

### Table "Premiers" calculée :

___________________________________________________
  | v | x | = | + | - | * | / | ^ | ; | ( | ) |'|'|
--|---|---|---|---|---|---|---|---|---|---|---|---|
S | o |   |   |   |   |   |   |   |   |   |   |   |
--|---|---|---|---|---|---|---|---|---|---|---|---|
A | o | o |   |   |   |   |   |   |   | o |   | o |
--|---|---|---|---|---|---|---|---|---|---|---|---|
E |   | o |   |   |   |   |   |   |   | o |   | o |
--|---|---|---|---|---|---|---|---|---|---|---|---|
F |   | o |   |   |   |   |   |   |   | o |   | o |
--|---|---|---|---|---|---|---|---|---|---|---|---|
O |   |   |   | o | o | o | o | o |   |   |   |   |
---------------------------------------------------

### Table "Suivants" calculée :

___________________________________________________
  | v | x | = | + | - | * | / | ^ | ; | ( | ) |'|'|
--|---|---|---|---|---|---|---|---|---|---|---|---|
S |   |   |   |   |   |   |   |   |   |   |   |   |
--|---|---|---|---|---|---|---|---|---|---|---|---|
A |   |   |   |   |   |   |   |   |   |   | o |   |
--|---|---|---|---|---|---|---|---|---|---|---|---|
E |   |   |   | o | o | o | o | o |   |   |   | o |
--|---|---|---|---|---|---|---|---|---|---|---|---|
F |   |   |   |   |   |   |   |   |   |   |   |   |
--|---|---|---|---|---|---|---|---|---|---|---|---|
O |   | o |   |   |   |   |   |   |   | o |   | o |
---------------------------------------------------

### Table LL1 calculée

___________________________________________________
  | v | x | = | + | - | * | / | ^ | ; | ( | ) |'|'|
--|---|---|---|---|---|---|---|---|---|---|---|---|
S | R1|   |   |   |   |   |   |   |   |   |   |   |
--|---|---|---|---|---|---|---|---|---|---|---|---|
A | R2| R3|   |   |   |   |   |   |   | R3|   | R3|
--|---|---|---|---|---|---|---|---|---|---|---|---|
E |   | R5|   |   |   |   |   |   |   | R5|   | R5|
--|---|---|---|---|---|---|---|---|---|---|---|---|
F |   | R6|   |   |   |   |   |   |   | R7|   | R8|
--|---|---|---|---|---|---|---|---|---|---|---|---|
O |   |   |   | R9|R10|R11|R12|R13|   |   |   |   |
---------------------------------------------------

## Difficultés rencontrées

### 1er problème:
Lors de la rédaction du programme d'analyse selon le principe d'analyse
descendante récursive je me suis confronté au problème suivant :
Pour les règles de E :
R4 : E -> E O F
R5 : E -> F
R6 : F -> x
R7 : F -> ( A )
R8 : F -> | E |
On voit bien ici que pour expliciter le cas ou E -> F, il suffit de regarder si
le caractère courant est soit 'x', '(' ou '|'.
Dans le cas ou a (2 + 5), on veut alors utiliser la règle E -> E O F.
Cependant ce cas la reprend les même conditions que pour E -> F.
Je ne vois donc pas comment distinguer ces deux règles en 2 fonctions distinctes.

### 2ème problème:
Après réflexion je pense que ma grammaire n'est pas correct.
Cette partie la de la grammaire comme présenté dans le 1er problème ci-dessus
pose problème :
R4 : E -> E O F
R5 : E -> F
On a une récursivité à gauche qu'il faut supprimer.
Je pense que j'aurais plutôt modifier ma grammaire comme ceci:
E -> F E'
E' -> O F E'
E' -> ε
Malheureusement par manque de temps je ne pourrais pas mettre en place cette
grammaire dans mon programme.

## Remarques éventuelles

Il m'a été impossible d'implémenter mon langage spécifique étant donné que le
caractère '|' n'est pas connu de la table des symboles.
J'ai cependant bien implémenter celui-ci dans ma grammaire visible dans la
partie "Grammaires utilisée pour les différentes étapes" du compte-rendu.
