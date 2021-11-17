//
// Created by Gilles Sérasset on 09/10/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tokens.h"
#include "lookahead_lexer.h"
#include "uthash.h"

#include "assignations.h"


/*********** Managing Symbol Table with UTHash Hash table ***************/

struct Symbol {
    float value;
    char var[100];     /* we'll use this field as the key */
    UT_hash_handle hh; /* makes this structure hashable */
};

struct Symbol *symbols = NULL;

void set_value(char var[], float val) {
    struct Symbol* symbol;
    HASH_FIND_STR( symbols, var, symbol );
    if (symbol == NULL) {
        // the symbol does not exist yet
        symbol = calloc(1, sizeof(struct Symbol));
        strcpy(symbol->var, var);
        HASH_ADD_STR( symbols, var, symbol );
    }
    symbol->value = val;
}

float get_value(char var[]) {
    struct Symbol* symbol;
    HASH_FIND_STR( symbols, var, symbol );
    if (symbol == NULL) {
        return 0.;
    } else {
        return symbol->value;
    }
}

void print_symbols() {
    struct Symbol *s;

    for(s=symbols; s != NULL; s=s->hh.next) {
        fprintf(stdout, "%s = %f\n", s->var, s->value);
    }
}

/************** Grammaire *****************/

/*
O -> +
O -> -
O -> *
O -> /
O -> ^
*/
void O() {
  if (currentToken()->type == PLUS || currentToken()->type == MINUS || currentToken()->type == MULT || currentToken()->type == DIV || currentToken()->type == EXPON) {
    next();
  } else {
    fprintf(stderr, "Erreur : opérateur attendu.\n");
  }
}

/*
F -> x
F -> ( A )
F -> | E |
*/
void F() {
  if(currentToken()->type == NUMBER) {
    next();
  } else if(currentToken()->type == LB) {
    next();
    A();
    if (currentToken()->type == RB) {
      next();
    } else {
      fprintf(stderr, "Erreur : ')' attendu.\n");
    }

  // Normalement on ajoute un else if ou on prend en compte le cas si on a le
  // caractère '|' pour les valeurs absolues. Cependant il n'est pas connu de
  // la table des symboles.
  } else {
    fprintf(stderr, "Erreur : '(' ou '|' attendu.\n");
  }
}

/*
E -> E O F
E -> F
*/
void E() {

}

/*
A -> v = A
A -> E
*/
void A() {
  if(currentToken()->type == VAR) {
    next();
    if(currentToken()->type == ASSIGN) {
      next();
      A();
    } else {
      fprintf(stderr, "Erreur : '=' attendu.\n");
    }

  // Normalement dans le else if on prend en compte le cas si on a le caractère
  // '|' pour les valeurs absolues. Cependant il n'est pas connu de la table
  // des symboles.
  } else if(currentToken()->type == NUMBER || currentToken()->type == LB ) {
    E();
  } else {
    fprintf(stderr, "Erreur : variable, nombre, '(' ou '|' attendu.\n");
  }
}

/*
S -> v = A ;
*/
void S() {
  if(currentToken()->type == VAR) {
    next();
    if(currentToken()->type == ASSIGN) {
      next();
      A();
      if (currentToken()->type == SEMICOLON) {
        next();
      } else {
        fprintf(stderr, "Erreur : ';' attendu.\n");
      }
    } else {
      fprintf(stderr, "Erreur : '=' attendu.\n");
    }
  } else {
    fprintf(stderr, "Erreur : variable attendu.\n");
  }
}





/*********** PROGRAMME PRINCIPAL ************/
int main(int argc, char *argv[]) {
    // fprintf(stderr, "Lexing from %s.\n", argv[1]);
    initLexer(argv[1]);
    // APPEL A LA SOURCE DE LA GRAMMAIRE
    S();
    // ON VERIFIE QUE LA GRAMMAIRE A BIEN TERMINE SON TRAVAIL A LA FIN DU MOT A ANALYSER
    if (currentToken() != NULL) {
        fprintf(stderr, "Unexpected input after assignation.\n");
        fprintf(stdout, "!!! ERREUR !!!\n");
        return 1;
    }
    // ET ON AFFICHE LA TABLE DES SYMBOLES
    print_symbols();
    return 0;
}
