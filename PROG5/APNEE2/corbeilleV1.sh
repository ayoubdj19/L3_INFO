#!/bin/bash

if [[ ! -d $HOME/.corbeille ]]
then
  mkdir $HOME/.corbeille
fi

for p in $@
do
  if [[ -f $p ]]
  then
    mv $p $HOME/.corbeille
  fi
done
