#!/bin/sh

for f in test/group/*.cpp
do
  echo $f
  cp $f test.cpp
  make clean -s
  make -s
  ./test.exe > test.out
  diff -y --suppress-common-lines -W 80 test/group/`basename $f .cpp`.refout test.out
  rm -f test.out
  echo ----
done

