#!/bin/bash

# Variables des compteurs de programmes corrects et incorrects
cpOK=0
cpXX=0

# Variable booleene :
# si programme correct alors isOK = 0, sinon isOK = 1
isOK=0

# Variables de comparaisons des résultats obtenus pour les programmes des étudiants
BON_P="Bon parenthesage"
MAUVAIS_P="Mauvais parenthesage"
i=0

declare -A tab

# Boucle pour vérifier chaque programmes des 500 étudiants
for file in programs/*
do
  isOK=0
  ((i+=1))

  # Affichage en temps réel du nombre de programmes d'étudiants traités
  printf "Programmes vérifiés : $i / 500\r"

  # Boucle des tests corrects créer manuellement
  for testNK in fichiers_test/parenthesage_correct/*
  do
    ./$file <$testNK >resultat
    resfile=$(cat resultat)
    if [[ $resfile !=  $BON_P ]]
    then
      isOK=1
      ((tab[$testNK]+=1))
      break
    fi
  done

  if [ $isOK == 0 ]
  then

    # Boucle des tests incorrects créer manuellement
    for testNK in fichiers_test/parenthesage_incorrect/*
    do
      ./$file <$testNK >resultat
      resfile=$(cat resultat)
      if [[ $resfile !=  $MAUVAIS_P ]]
      then
        isOK=1
        ((tab[$testNK]+=1))
        break
      fi
    done

    if [ $isOK == 0 ]
    then

      # Boucle des tests corrects générés par algorithme
      for testNK in fichiers_test_generes/parenthesage_correct/*
      do
        ./$file <$testNK >resultat
        resfile=$(cat resultat)
        if [[ $resfile !=  $BON_P ]]
        then
          isOK=1
          ((tab[$testNK]+=1))
          break
        fi
      done



      if [ $isOK == 0 ]
      then

        # Boucle des tests incorrects générés par algorithme
        for testNK in fichiers_test_generes/parenthesage_incorrect/*
        do
          ./$file <$testNK >resultat
          resfile=$(cat resultat)
          if [[ $resfile !=  $MAUVAIS_P ]]
          then
            isOK=1
            ((tab[$testNK]+=1))
            break
          fi
        done
      fi
    fi
  fi

  # Incrémentation des compteurs du nombre de programmes corrects et incorrects
  if [ $isOK == 0 ]
  then
    ((cpOK+=1))
  else
    ((cpXX+=1))
  fi

done

# Affichage du nombre de programmes incorrects par test généré manuellement
printf "\nNombre de programme qui ne réussise pas le test :\n"
for i in "${!tab[@]}"
do
  j=${i#*fichiers_test/parenthesage_incorrect/}
  j=${j#*fichiers_test/parenthesage_correct/}
  printf "\t$j = ${tab[$i]}\n"
done

# Affichage du nombre de programmes corrects et incorrects
printf "\nProgrammes correct = $cpOK\nProgrammes incorrect = $cpXX\n"
