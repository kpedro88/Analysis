#!/bin/bash

FORCE=""

#check arguments
while getopts "f" opt; do
  case "$opt" in
  k) FORCE="force"
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
)

for MACRO in ${MACROS[@]}; do
  if [ -z "$FORCE" ]; then
    root -b -l -q ${MACRO}.C+
  else
    root -b -l -q ${MACRO}.C++
  fi
done
