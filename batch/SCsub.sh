#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_scan.txt
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
STORE=${INDIR}/scan
CHECKARGS=""

#check arguments
while getopts "k" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

source exportScan.sh

for SAMPLE in ${SAMPLES[@]}
  do
    ./SCtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${INDIR} ${STORE}
  done

