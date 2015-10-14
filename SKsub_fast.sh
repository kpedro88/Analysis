#!/bin/bash

KEEPTAR=$1
JOBDIR=batch
INPUT=input/input_selection.txt
SELTYPE=signal
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV2/scan
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV2

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

source exportFast.sh

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done

