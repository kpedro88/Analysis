#!/bin/bash

source exportProd.sh

KEEPTAR=$1
JOBDIR=jobs
INPUT=input/input_scan.txt
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV4
STORE=${INDIR}/scan

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

source exportScan.sh

for SAMPLE in ${SAMPLES[@]}
  do
    ./SCtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${INDIR} ${STORE}
  done

