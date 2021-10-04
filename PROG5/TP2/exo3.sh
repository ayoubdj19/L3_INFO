#!/bin/bash
res=$(cat /etc/passwd | cut -d':' -f 1,3)
i=0
for p in $res
do
  cutted=$(echo $p | cut -d':' -f2)
  if [[ $cutted -ge 1000 ]]
  then
    echo $p
  fi
done
