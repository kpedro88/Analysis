#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE=signal,signal_JECup,signal_JECdown,LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,GJetLDP_CleanVars,GJet_CleanVarsGJ,GJetLDP_CleanVarsGJ
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan
CHECKARGS=""

#check arguments
while getopts "k" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

source exportFast.sh

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done

