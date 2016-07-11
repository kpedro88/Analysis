#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE=signal,signal_JECup,signal_JECdown,signal_JERup,signal_JERdown
SELTYPE2=LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,GJetLDP_CleanVars
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan
CHECKARGS=""
EXPORT=exportFast.sh

#check arguments
while getopts "kg" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  g) INPUT=input/input_selection_genMHT.txt
     STORE=${STORE}/genMHT
	 EXPORT=exportFastT2tt.sh
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

source ${EXPORT}

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE2} ${INDIR} ${OUTDIR} ${STORE}
  done

