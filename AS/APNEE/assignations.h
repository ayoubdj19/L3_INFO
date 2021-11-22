//
// Created by Gilles Sérasset on 09/10/2019.
//

#ifndef SIMPLE_ASSIGN_ASSIGNATIONS_H
#define SIMPLE_ASSIGN_ASSIGNATIONS_H

/*
S -> v = A ;
*/
void S();

/*
A -> v = A
A -> E
*/
void A();

/*
E -> E O F
E -> F
*/
void E();

/*
F -> x
F -> ( A )
F -> | E |
*/
void F();

/*
O -> +
O -> -
O -> *
O -> /
O -> ^
*/
void O();

#endif //SIMPLE_ASSIGN_ASSIGNATIONS_H
