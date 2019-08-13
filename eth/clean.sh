#!/bin/bash



if [ $# == 0 ]; then
  echo "?? who to clean ?"
  exit
fi

if [ "$1" == '-a' ]; then
  echo "Cleaning up {entity}-info.txt"
  rm *-info.txt
  exit
fi

ENT=$1

rm $ENT/$ENT.log
rm -rf $ENT/geth
