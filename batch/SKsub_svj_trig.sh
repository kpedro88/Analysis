#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection_svj.txt
SELTYPE=dijetmtmutrig,dijetmtdetamutrig
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
CHECKARGS=""

#check arguments
while getopts "k" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

#for year in 2016 2017; do
for year in 2016; do
  source exportSkimData${year}.sh
  for SAMPLE in ${SAMPLES[@]}; do
    if [[ $SAMPLE == *"SingleMuon"* ]]; then
      ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
    fi
  done
done
