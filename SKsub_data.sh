#!/bin/bash

KEEPTAR=$1
JOBDIR=batch
INPUT=input/input_selection.txt
SELTYPE=signal,LDP,SLm,SLe,GJet_CleanVars,DYm_CleanVars,DYe_CleanVars,GJetLDP_CleanVars
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV3
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV3

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

SAMPLES=(
DoubleEG_2015C \
DoubleMuon_2015C \
HTMHT_2015C \
JetHT_2015C \
MET_2015C \
SingleElectron_2015C \
SingleMuon_2015C \
SinglePhoton_2015C \
DoubleEG_re2015D \
DoubleMuon_re2015D \
HTMHT_re2015D \
JetHT_re2015D \
MET_re2015D \
SingleElectron_re2015D \
SingleMuon_re2015D \
SinglePhoton_re2015D \
DoubleEG_2015Db \
DoubleMuon_2015Db \
HTMHT_2015Db \
JetHT_2015Db \
MET_2015Db \
SingleElectron_2015Db \
SingleMuon_2015Db \
SinglePhoton_2015Db
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done
