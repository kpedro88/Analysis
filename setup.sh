#!/bin/bash

for i in scripts/*.*; do
  j=`basename $i`
  ln -s -f $i $j
done
