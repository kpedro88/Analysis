#!/bin/bash

source exportProd.sh

KEEPTAR=$1
JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE=signal,LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,DYm_CleanVars,DYe_CleanVars
SETYPE2=GJetLDP_CleanVars,DYmLDP_CleanVars,DYeLDP_CleanVars,GJet_ManualCleanVars,GJetLDP_ManualCleanVars
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

SAMPLES=(
DYJetsToLL_M-50_HT-100to200 \
DYJetsToLL_M-50_HT-200to400 \
DYJetsToLL_M-50_HT-400to600 \
DYJetsToLL_M-50_HT-600toInf \
DYJetsToLL_M-50 \
GJets_HT-100to200 \
GJets_HT-200to400 \
GJets_HT-400to600 \
GJets_HT-600toInf \
ZJetsToNuNu_HT-100to200 \
ZJetsToNuNu_HT-200to400 \
ZJetsToNuNu_HT-400to600 \
ZJetsToNuNu_HT-600toInf \
QCD_HT-200to300 \
QCD_HT-300to500 \
QCD_HT-500to700 \
QCD_HT-700to1000 \
QCD_HT-1000to1500 \
QCD_HT-1500to2000 \
QCD_HT-2000toInf \
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
WJetsToLNu_HT-2500toInf \
WJetsToLNu_HT-1200to2500 \
WJetsToLNu_HT-800to1200 \
WJetsToLNu_HT-600to800 \
WJetsToLNu_HT-400to600 \
WJetsToLNu_HT-200to400 \
WJetsToLNu_HT-100to200 \
ST_s-channel \
ST_t-channel_top \
ST_t-channel_antitop \
ST_tW_top \
ST_tW_antitop \
WWToLNuQQ \
WWTo2L2Nu \
WWTo1L1Nu2Q \
ZZTo2Q2Nu \
WZTo1L3Nu \
WZTo1L1Nu2Q \
ZZTo2L2Q \
WH_HToBB_WToLNu_M125 \
ZH_HToBB_ZToNuNu_M125 \
ggZH_HToBB_ZToNuNu_M125 \
TTWJetsToQQ \
TTWJetsToLNu \
TTZToQQ \
TTZToLLNuNu \
ttHJetTobb_M125 \
ttHJetTobb_M125_ext1 \
ttHJetTobb_M125_ext2 \
ttHJetTobb_M125_ext3 \
TTTT \
TTTT_ext1 \
WWZ \
ZZZ \
WZZ \
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE2} ${INDIR} ${OUTDIR} ${STORE}
  done
