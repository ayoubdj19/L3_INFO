#!/bin/bash

cpOK=0
cpXX=0
isOK=0
BON_P="Bon parenthesage"
BON="Bon"
MAUVAIS_P="Mauvais parenthesage"
MAUVAIS="Mauvais"

declare -A tab
for file in programs/*
do
  isOK=0

  # for testOK in ../fichiers_test_generes/parenthesage_correct/*
  # do
  #   ./$file <$testOK >resultat
  #   resfile=$(cat resultat)
  #   if ["$resfile" !=  "$BON_P" && "$resfile" ==  "$BON"]
  #   then
  #     isOK=1
  #     ((tab[$testOK]+=1))
  #   fi
  # done

  if [ $isOK == 0 ]
  then
    for testNK in ~/Documents/GitHub/L3_INFO/ALGO5/APNEE2/fichiers_test_generes/parenthesage_incorrect_trop_parentheses_fermantes/*
    do

      ./$file <$testNK >resultat
      resfile=$(cat resultat)
      if ["$resfile" !=  "$MAUVAIS_P" && "$resfile" !=  "$MAUVAIS"]
      then
        isOK=1
        ((tab[$testNK]+=1))
      fi
    done
  fi

  if [ $isOK == 0 ]
  then
    ((cpOK+=1))
  else
    ((cpXX+=1))
  fi

done
printf "\nNombre de programme qui ne réussise pas le test :\n"
for i in "${!tab[@]}"
do
  # j=${i#*fichiers_test/parenthesage_incorrect/}
  # j=${j#*fichiers_test/parenthesage_correct/}
  printf "\t$j = ${tab[$i]}\n"
done
printf "\nProgrammes correct = $cpOK\nProgrammes incorrect = $cpXX\n"
