#!/bin/bash

source exportProd.sh

KEEPTAR=$1
JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE=signal,signalUnblind,LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,DYm_CleanVars,DYe_CleanVars,GJetLDP_CleanVars,DYmLDP_CleanVars,DYeLDP_CleanVars
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

SAMPLES=(
DoubleEG_re2015C \
DoubleMuon_re2015C \
HTMHT_re2015C \
JetHT_re2015C \
MET_re2015C \
SingleElectron_re2015C \
SingleMuon_re2015C \
SinglePhoton_re2015C \
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
