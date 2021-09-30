#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "PileTypeParentheses.h"

PileTypeParentheses* NouvellePileTypeParentheses(void)
{
   PileTypeParentheses* nouvellePile = malloc(sizeof(PileTypeParentheses));

   nouvellePile->tete = NULL;
   
   return nouvellePile;
}

ElementPileTypeParentheses* NouvelElementPileTypeParentheses(void)
{
   ElementPileTypeParentheses* nouvelElement = malloc(sizeof(ElementPileTypeParentheses));

   nouvelElement->typeParenthese = NON_DEFINI;
   nouvelElement->profondeur = 0;
   nouvelElement->suivant = NULL;
   
   return nouvelElement;
}


void DetruirePileTypeParentheses(PileTypeParentheses* pileADetruire)
{
    if(pileADetruire != NULL)
    {
         if(pileADetruire->tete != NULL)
         {
            DetruireElementPileTypeParenthesesEtSaDescendance(pileADetruire->tete);
         }

         free(pileADetruire);
         pileADetruire = NULL;
    }
}

void DetruireElementPileTypeParenthesesEtSaDescendance(ElementPileTypeParentheses* elementADetruire)
{
   if(elementADetruire != NULL)
   {
      if(elementADetruire->suivant != NULL)
      {
         DetruireElementPileTypeParenthesesEtSaDescendance(elementADetruire->suivant);
      }

      DetruireElementPileTypeParentheses(elementADetruire);
      elementADetruire = NULL;
   }
}

void DetruireElementPileTypeParentheses(ElementPileTypeParentheses* elementADetruire)
{
   if(elementADetruire != NULL)
   {
      free(elementADetruire);
      elementADetruire = NULL;
   }
}


int Depiler(PileTypeParentheses* pileADepiler, TYPES_PARENTHESES* typeParentheseElementDepile, int* profondeurParentheseElementDepile)
{
    int retour = 1;
    
    if(EstPileVide(pileADepiler) == 1)
    {
        retour = 0;
    }
    else
    {
        ElementPileTypeParentheses* ElementDepile = pileADepiler->tete;
        
        *typeParentheseElementDepile = ElementDepile->typeParenthese;
        *profondeurParentheseElementDepile = ElementDepile->profondeur;
        pileADepiler->tete = ElementDepile->suivant;
        
        DetruireElementPileTypeParentheses(ElementDepile);
    }
    
    return retour;
}

void Empile(PileTypeParentheses* pileAEmpiler, TYPES_PARENTHESES typeParentheseElementEmpiler)
{
   //Si la pile n'est pas vide
   if(EstPileVide(pileAEmpiler) == 0)
   {
      ElementPileTypeParentheses* elementEmpile = NouvelElementPileTypeParentheses();
      elementEmpile->typeParenthese = typeParentheseElementEmpiler;
      elementEmpile->profondeur = pileAEmpiler->tete->profondeur + 1;
      elementEmpile->suivant = pileAEmpiler->tete;
      pileAEmpiler->tete = elementEmpile;
   }
   //Si la pile est vide
   else
   {
      ElementPileTypeParentheses* elementEmpile = NouvelElementPileTypeParentheses();
      elementEmpile->typeParenthese = typeParentheseElementEmpiler;
      elementEmpile->profondeur = 0;
      pileAEmpiler->tete = elementEmpile;
   }
}

int EstPileVide(PileTypeParentheses* pileAControler)
{
    int retour = 0;
    
    if(pileAControler->tete == NULL)
    {
        retour = 1;
    }
    
    return retour;
}

int InfosSommetPile(PileTypeParentheses* laPile, TYPES_PARENTHESES* typeParentheseSommet, int* profondeurParentheseSommet)
{
   int retour = 0;

   if(EstPileVide(laPile) == 0)
   {
      *typeParentheseSommet = laPile->tete->typeParenthese;
      *profondeurParentheseSommet = laPile->tete->profondeur;

      retour = 1;
   }

   return retour;
}