#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection.txt
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

# selections for DY
SELTYPE=DYm_CleanVarsDY,DYe_CleanVarsDY,DYmLDP_CleanVarsDY,DYeLDP_CleanVarsDY
SAMPLES=(
DYJetsToLL_M-50_HT-100to200 \
DYJetsToLL_M-50_HT-200to400 \
DYJetsToLL_M-50_HT-400to600 \
DYJetsToLL_M-50_HT-600toInf \
TTJets \
TTJets_SingleLeptFromT \
TTJets_SingleLeptFromT_ext1 \
TTJets_SingleLeptFromTbar \
TTJets_SingleLeptFromTbar_ext1 \
TTJets_DiLept \
TTJets_DiLept_ext1 \
TTJets_HT-600to800 \
TTJets_HT-800to1200 \
TTJets_HT-1200to2500 \
TTJets_HT-2500toInf \
TTZToLLNuNu \
SingleElectron_re2015C \
SingleElectron_re2015D \
SingleElectron_2015Db \
SingleMuon_re2015C \
SingleMuon_re2015D \
SingleMuon_2015Db \
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done

# selections for GJ
SELTYPE=GJet_CleanVarsGJ,GJetLDP_CleanVarsGJ,GJet_CleanVarsGJloose,GJetLDP_CleanVarsGJloose
SAMPLES=(
GJets_HT-100to200 \
GJets_HT-200to400 \
GJets_HT-400to600 \
GJets_HT-600toInf \
QCD_HT-200to300 \
QCD_HT-300to500 \
QCD_HT-500to700 \
QCD_HT-700to1000 \
QCD_HT-1000to1500 \
QCD_HT-1500to2000 \
QCD_HT-2000toInf \
SinglePhoton_re2015C \
SinglePhoton_re2015D \
SinglePhoton_2015Db \
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done
