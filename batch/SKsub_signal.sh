#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE=signal,signal_JECup,signal_JECdown,signal_JERup,signal_JERdown,LDP,SLm,SLe,SLmLDP,SLeLDP
SELTYPE2=GJet_CleanVars,DYm_CleanVars,DYe_CleanVars,GJetLoose_CleanVars,GJetLDP_CleanVars,GJetLooseLDP_CleanVars,DYmLDP_CleanVars,DYeLDP_CleanVars
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}
CHECKARGS=""

#check arguments
while getopts "k" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

source exportSkimSignal.sh

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE2} ${INDIR} ${OUTDIR} ${STORE}
  done
