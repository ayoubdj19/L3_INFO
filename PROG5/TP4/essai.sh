#!/bin/bash
i=0
e=0
for p in {1..1000}
do
  echo $p
  rand=$RANDOM%2
  if ( [[ $rand == 0 ]] && [[ $i < 666 ]] ) || [[$e == 333]]; then
    echo "i $RANDOM $RANDOM" >>jeu_test.txt
  else
    echo "e" >>jeu_test.txt
  fi
done
