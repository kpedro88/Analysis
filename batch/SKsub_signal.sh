#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE=signal,signal_JECup,signal_JECdown,LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,DYm_CleanVars,DYe_CleanVars
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

SAMPLES=(
T1bbbb_1000_900 \
T1bbbb_1500_100 \
T1qqqq_1000_800 \
T1qqqq_1400_100 \
T1tttt_1200_800 \
T1tttt_1500_100 \
T2tt_500_325 \
T2tt_850_100 \
T5HH_1200_200 \
T5HH_1200_950 \
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE2} ${INDIR} ${OUTDIR} ${STORE}
  done
