#!/bin/bash

source exportProd.sh

KEEPTAR=$1
JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/${RUN2PRODV}

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

#first mc
INPUT=input/input_selection.txt
SELTYPE=SLe2_CleanVars,SLm2_CleanVars

SAMPLES=(
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
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done

# now data (w/ HLT)
SELTYPE=SLm2_CleanVars
SAMPLES=(
SingleMuon_re2015C \
SingleMuon_re2015D \
SingleMuon_2015Db \
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done

SELTYPE=SLe2_CleanVars
SAMPLES=(
SingleElectron_re2015C \
SingleElectron_re2015D \
SingleElectron_2015Db \
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done
