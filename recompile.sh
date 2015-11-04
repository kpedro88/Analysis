#!/bin/bash

FORCE=$1

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
