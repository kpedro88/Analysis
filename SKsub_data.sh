#!/bin/bash

KEEPTAR=$1
JOBDIR=batch
INPUT=input/input_selection.txt
SELTYPE=signal,LDP,SLm,SLe,GJet_CleanVars,DYm_CleanVars,DYe_CleanVars,GJetLDP_CleanVars
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV2
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV2

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

SAMPLES=(
DoubleEG_2015B \
DoubleMuon_2015B \
HTMHT_2015B \
SingleElectron_2015B \
SingleMuon_2015B \
SinglePhoton_2015B \
DoubleEG_re2015B \
DoubleMuon_re2015B \
HTMHT_re2015B \
SingleElectron_re2015B \
SingleMuon_re2015B \
SinglePhoton_re2015B \
DoubleEG_2015C \
DoubleMuon_2015C \
HTMHT_2015C \
JetHT_2015C \
MET_2015C \
SingleElectron_2015C \
SingleMuon_2015C \
SinglePhoton_2015C \
DoubleEG_2015D \
DoubleMuon_2015D \
HTMHT_2015D \
JetHT_2015D \
MET_2015D \
SingleElectron_2015D \
SingleMuon_2015D \
SinglePhoton_2015D
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done
