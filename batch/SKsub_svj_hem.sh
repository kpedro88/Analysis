#!/bin/bash

RUN2PRODV=Run2ProductionV16a

JOBDIR=jobs
INPUT=input/input_svj_skim_hem.txt
SELTYPE=HEMpresel
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

SAMPLES=(
JetHT_2018B \
JetHT_2018BHEM \
JetHT_2018BHEMmiti \
)

for SAMPLE in ${SAMPLES[@]}; do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
done
