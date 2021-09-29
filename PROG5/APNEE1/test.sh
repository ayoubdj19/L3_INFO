#!/bin/bash
resultat_attendu="Bon parenthesage"

#max=ls programs/ | wc -l
cpOK=0
cpXX=0
isOK=0
for file in programs/*
do
  ./$file <fichiers_test/fichier_vide.txt >resultat
  if diff resultat resultat_attendu
  then
    ((cpOK+=1))
    # echo OK
  else
    ((cpXX+=1))
    # echo XXXX
  fi
done
printf "\nProgrammes correct = $cpOK\nProgrammes incorrect = $cpXX\n"
