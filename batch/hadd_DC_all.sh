#!/bin/bash

#initialize parameters
RUN=0
ARGS=""

#check arguments
while getopts "r" opt; do
  case "$opt" in
  r) RUN=1
     ARGS="${ARGS} -r"
    ;;
  esac
done

for i in T1tttt T1bbbb T2tt T2bb T1qqqq T5qqqqVV T2qq; do
  ./hadd_DC.sh -x $i ${ARGS}
done
