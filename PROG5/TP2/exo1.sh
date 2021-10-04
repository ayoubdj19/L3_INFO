#!/bin/bash

for p in $@
do
  ls -l | grep $p | head -c 10
  printf " $p\n"
done
