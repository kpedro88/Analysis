#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}
SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,btagCFuncUp,btagCFuncDown,ctagCFuncUp,ctagCFuncDown,mistagCFuncUp,mistagCFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown
VARS=JECup,JECdown,JERup,JERdown,genMHT
CHECKARGS=""
SUFFIX=""

#check arguments
while getopts "kx:" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  x) SUFFIX=$OPTARG
    ;;
  esac
done

if [ -n "$SUFFIX" ]; then
  source exportFast${SUFFIX}.sh
else
  source exportFast.sh
fi

./SKcheck.sh ${CHECKARGS}

for SAMPLE in ${SAMPLES[@]}; do
  ./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS} ${VARS} ${STORE} ${SAMPLE}
done
