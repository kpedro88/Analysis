#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}
SYSTS=nominal,QCD,puunc,pdfunc,scaleunc
SYSTS2=isrunc,trigStatUnc,trigSystUnc,JEC
SYSTS3=btagSFunc,ctagSFunc,mistagSFunc,btagCFunc,ctagCFunc,mistagCFunc
CONTAMS=LDP,GJet_CleanVars,GJetLDP_CleanVars,GJet_CleanVarsGJ,GJetLDP_CleanVarsGJ
CHECKARGS=""
SUFFIX="Mixed"

#check arguments
while getopts "kx:" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  x) SUFFIX=$OPTARG
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

nparts=`ls -1 ../input/fast${SUFFIX}/input_sets_DC_fast${SUFFIX}_*.txt | wc -l`
nparts=$(($nparts-1))

echo "Submitting parts 0.."$nparts

for PART in $(seq 0 $nparts); do
  ./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS} none ${PART} ${STORE}
  ./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS2} none ${PART} ${STORE}
  ./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS3} none ${PART} ${STORE}
  ./DCtemp.sh ${JOBDIR} ${INDIR} none ${CONTAMS} ${PART} ${STORE}
done
