#!/bin/bash

for i in scripts/*.*; do
  j=`basename $i`
  ln -s $i $j
done
