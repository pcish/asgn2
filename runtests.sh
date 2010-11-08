#!/bin/sh

for f in testcases/group/*.cpp
do
  echo $f
  cp $f test.cpp
  make clean -s
  make -s
  ./test.o > test.out
  diff -y --suppress-common-lines -W 80 testcases/group/`basename $f .cpp`.refout test.out
  rm -f test.out
  echo ----
done

