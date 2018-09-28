#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE=signal,LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,DYm_CleanVars,DYe_CleanVars
SELTYPE2=GJetLoose_CleanVars,GJetLDP_CleanVars,GJetLooseLDP_CleanVars,DYmLDP_CleanVars,DYeLDP_CleanVars
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

for year in 2016 2017 2018; do
  source exportSkimData${year}.sh

  for SAMPLE in ${SAMPLES[@]}; do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE2} ${INDIR} ${OUTDIR} ${STORE}
  done
done
