#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}
SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,btagCFuncUp,btagCFuncDown,ctagCFuncUp,ctagCFuncDown,mistagCFuncUp,mistagCFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown,puaccuncUp,puaccuncDown
VARS=JECup,JECdown,JERup,JERdown,genMHT
CHECKARGS=""
SUFFIX=""
SKIPFILE=""

#check arguments
while getopts "kx:s:" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  x) SUFFIX=$OPTARG
    ;;
  s) SKIPFILE=$OPTARG
  esac
done

if [ -n "SKIPFILE" ]; then
  source ${SKIPFILE}
fi

if [ -n "$SUFFIX" ]; then
  source exportFast${SUFFIX}.sh
else
  source exportFast.sh
fi

./SKcheck.sh ${CHECKARGS}

for SAMPLE in ${SAMPLES[@]}; do
  # check skips
  SKIPTHIS=""
  for SKIP in ${SKIPLIST[@]}; do
    if [[ $SAMPLE == "$SKIP"* ]]; then
      SKIPTHIS=yes
      break
    fi
  done
  if [[ -n "$SKIPTHIS" ]]; then
    continue
  fi
  
  ./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS} ${VARS} ${STORE} ${SAMPLE}
done
