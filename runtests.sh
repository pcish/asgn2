#!/bin/sh

if [ $# -ge "1" ]
then
  folder=$1""/
else
  folder=group/
fi 

exename=test
if uname | grep -iq cygwin
then
  exename=test.exe
fi

for f in testcases/$folder*.cpp
do
  echo $f
  cp $f test.cpp
  make clean -s
  make test -s
  if [ -e testcases/$folder`basename $f .cpp`.refout ]
  then
    ./$exename > test.out
    diff -y --suppress-common-lines -W 80 testcases/$folder`basename $f .cpp`.refout test.out
    rm -f test.out
  else
    ./$exename
  fi
  echo ----
done

