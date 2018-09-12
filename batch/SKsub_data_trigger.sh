#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection_trigger.txt
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

for year in 2016 2017; do
  source exportSkimData${year}.sh

  for SAMPLE in ${SAMPLES[@]}; do
    if [[ $SAMPLE == *"SingleElectron"* ]]; then
      ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} SLeTrigger,SLeTrigger_JetLeptonClean ${INDIR} ${OUTDIR} ${STORE}
    elif [[ $SAMPLE == *"SingleMuon"* ]]; then
      ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} SLmTrigger,SLmTrigger_JetLeptonClean ${INDIR} ${OUTDIR} ${STORE}
    elif [[ $SAMPLE == *"MET"* ]]; then
      ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} signalTrigger,signalTrigger_JetLeptonClean ${INDIR} ${OUTDIR} ${STORE}
    elif [[ $SAMPLE == *"JetHT"* ]]; then
      ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} QCDTrigger,QCDTrigger_JetLeptonClean ${INDIR} ${OUTDIR} ${STORE}
    elif [[ $SAMPLE == *"SinglePhoton"* ]]; then
      ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} PhotonTrigger ${INDIR} ${OUTDIR} ${STORE}
    fi
  done
done
