#!/bin/bash

for p in $@
do
  i=16
  add=${#p}
  ((i+=${#p}))
  res=$(ls -l | grep $p | tail -c $i)
  ((add+=1))
  head=$(echo $res | head -c 7)
  tail=$(echo $res | tail -c $add)
  printf "$head$tail\n"
done
