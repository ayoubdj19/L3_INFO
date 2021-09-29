#!/bin/bash


#max=ls programs/ | wc -l
cpOK=0
cpXX=0
isOK=0
declare -A tab
for file in programs/*
do
  isOK=0

  echo "Bon parenthesage" >resultat_attendu
  for testOK in fichiers_test/parenthesage_correct/*
  do
    ./$file <$testOK >resultat
    if diff resultat resultat_attendu
    then
      isOK=1
      ((tab[$testOK]+=1))
    fi
  done

  echo "Mauvais parenthesage" >resultat_attendu
  if [ $isOK == 0 ]
  then
    for testNK in fichiers_test/parenthesage_incorrect/*
    do
      ./$file <$testNK >resultat
      if diff resultat resultat_attendu
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
  j=${i#*fichiers_test/parenthesage_incorrect/}
  j=${j#*fichiers_test/parenthesage_correct/}
  printf "\t$j = ${tab[$i]}\n"
done
printf "\nProgrammes correct = $cpOK\nProgrammes incorrect = $cpXX\n"
