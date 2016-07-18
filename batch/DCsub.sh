#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}
SYSTS=nominal,QCD,puunc,scaleunc,isrunc
SYSTS2=trigStatUnc,trigSystUnc,JEC,JER
SYSTS3=btagSFunc,mistagSFunc,btagCFunc,ctagCFunc,mistagCFunc
CONTAMS=signal_genMHT,LDP,LDP_genMHT,GJet_CleanVars,GJetLDP_CleanVars
CHECKARGS=""
SUFFIX=""

#check arguments
while getopts "kx:g" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  x) SUFFIX=$OPTARG
    ;;
  esac
done

if [ -n "$SUFFIX" ]; then
  STORE=${STORE}/${SUFFIX}
fi

./SKcheck.sh ${CHECKARGS}

nparts=`ls -1 ../input/fast${SUFFIX}/input_sets_DC_fast${SUFFIX}_*.txt | wc -l`
nparts=$(($nparts-1))

echo "Submitting parts 0.."$nparts

for PART in $(seq 0 $nparts); do
  ./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS} none ${PART} ${STORE} ${SUFFIX}
  ./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS2} none ${PART} ${STORE} ${SUFFIX}
  ./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS3} none ${PART} ${STORE} ${SUFFIX}
  ./DCtemp.sh ${JOBDIR} ${INDIR} none ${CONTAMS} ${PART} ${STORE} ${SUFFIX}
done
