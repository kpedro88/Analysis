#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection_supplementary.txt
SELTYPE=signal,signalMinusNJet,signalMinusHT,signalMinusMHT
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan/supplementary
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
T1tttt_1500_100_fast \
T1tttt_1200_800_fast \
T1bbbb_1500_100_fast \
T1bbbb_1000_900_fast \
T1qqqq_1400_100_fast \
T1qqqq_1000_800_fast \
T5qqqqVV_1400_100_fast \
T5qqqqVV_1000_800_fast \
T2tt_700_50_fast \
T2tt_300_200_fast \
T2bb_650_1_fast \
T2bb_500_300_fast \
T2qq_1000_100_fast \
T2qq_700_400_fast \
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done

