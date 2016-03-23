#!/bin/bash

FORCE=""

#check arguments
while getopts "f" opt; do
  case "$opt" in
  f) FORCE="force"
    ;;
  esac
done


MACROS=(
KSkimDriver \
KScanDriver \
KPlotDriver \
KPlotDriverDC \
KPlotDriverDCsyst \
MakeAllDCdata \
MakeAllDCsyst \
KCutflowDriver \
)

for MACRO in ${MACROS[@]}; do
  if [ -z "$FORCE" ]; then
    root -b -l -q ${MACRO}.C+
  else
    root -b -l -q ${MACRO}.C++
  fi
  
  RECOMPEXIT=$?
  if [[ $RECOMPEXIT -ne 0 ]]; then
    echo "exit code $RECOMPEXIT, compilation failure"
    exit $RECOMPEXIT
  fi
done
