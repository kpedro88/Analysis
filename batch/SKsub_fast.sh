#!/bin/bash

source exportProd.sh

KEEPTAR=$1
JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE=signal,signal_JECup,signal_JECdown,LDP,SLm,SLe,GJet_CleanVars
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/${RUN2PRODV}/scan

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

source exportFast.sh

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done

