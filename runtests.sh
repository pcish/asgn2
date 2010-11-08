#!/bin/sh

if [ $# -ge "1" ]
then
  folder=$1""/
else
  folder=group/
fi 

for f in testcases/$folder*.cpp
do
  echo $f
  cp $f test.cpp
  make clean -s
  make -s
  ./test.exe > test.out
  diff -y --suppress-common-lines -W 80 testcases/$folder`basename $f .cpp`.refout test.out
  rm -f test.out
  echo ----
done

