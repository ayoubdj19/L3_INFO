#!/bin/bash

if [[ ! -d $HOME/.corbeille ]]
then
  mkdir $HOME/.corbeille
fi

if [[ $# -ge 2 ]]
then
  for p in ${@:2}
  do
    case $1 in
      efface)
        mv $p $HOME/.corbeille
        ;;
      restaure)
        mv $HOME/.corbeille/$p .
        ;;
      info)
        if [[ -d $p ]]
        then
          ls -l $p
        elif [[ -d $HOME/.corbeille/$p ]]
        then
          ls -l $HOME/.corbeille/$p
        elif [[ -f $p ]]
        then
          ls -l $p
        elif [[ -f $HOME/.corbeille/$p ]]
        then
          ls -l $HOME/.corbeille/$p
        else
          echo "$p n'existe pas."
        fi
        ;;
      vide)
        rm $HOME/.corbeille/$p
        ;;
    esac
  done
elif [[ ($# == 1) && ($1 == "vide") ]]; then
  cd $HOME/.corbeille/
  rm *
fi
