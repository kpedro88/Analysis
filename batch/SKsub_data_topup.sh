#!/bin/bash

source exportProd.sh

KEEPTAR=$1
JOBDIR=jobs
INPUT=input/input_selection.txt
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/${RUN2PRODV}

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

SAMPLES=(
DoubleEG_2015Db \
DoubleMuon_2015Db \
HTMHT_2015Db \
JetHT_2015Db \
MET_2015Db \
SingleElectron_2015Db
SingleMuon_2015Db \
SinglePhoton_2015Db \
SinglePhoton_2015Db \
)
SELTYPES=(
signalUnblind,signal,SLmLooseLDP,SLmLoose,SLm,SLeLooseLDP,SLeLoose,SLe,LDP,GJet_CleanVars,GJetLDP_CleanVars,DYm_CleanVars,DYmLDP_CleanVars,DYe_CleanVars,DYeLDP_CleanVars \
signalUnblind,signal,SLmLooseLDP,SLmLoose,SLm,SLeLooseLDP,SLeLoose,SLe,LDP,GJet_CleanVars,GJetLDP_CleanVars,DYm_CleanVars,DYmLDP_CleanVars,DYe_CleanVars,DYeLDP_CleanVars \
signalUnblind,signal,SLmLooseLDP,SLmLoose,SLm,SLeLooseLDP,SLeLoose,SLe,LDP,GJet_CleanVars,GJetLDP_CleanVars,DYm_CleanVars,DYmLDP_CleanVars,DYe_CleanVars,DYeLDP_CleanVars \
signalUnblind,signal,SLmLooseLDP,SLmLoose,SLm,SLeLooseLDP,SLeLoose,SLe,LDP,GJet_CleanVars,GJetLDP_CleanVars,DYm_CleanVars,DYmLDP_CleanVars,DYe_CleanVars,DYeLDP_CleanVars \
signalUnblind,signal,SLmLooseLDP,SLmLoose,SLm,SLeLooseLDP,SLeLoose,SLe,LDP,GJet_CleanVars,GJetLDP_CleanVars,DYm_CleanVars,DYmLDP_CleanVars,DYe_CleanVars,DYeLDP_CleanVars \
signalUnblind,signal,SLmLooseLDP,SLmLoose,SLm,SLeLooseLDP,SLeLoose,SLe2_CleanVars,SLe,LDP,GJet_CleanVars,GJetLDP_CleanVars,DYm_CleanVars,DYmLDP_CleanVars,DYe_CleanVars,DYeLoose_CleanVars,DYeLDP_CleanVars \
signalUnblind,signal,SLmLooseLDP,SLmLoose,SLm2_CleanVars,SLm,SLeLooseLDP,SLeLoose,SLe,LDP,GJet_CleanVars,GJetLDP_CleanVars,DYm_CleanVars,DYmLoose_CleanVars,DYmLDP_CleanVars,DYe_CleanVars,DYeLDP_CleanVars \
signalUnblind,signal,SLmLooseLDP,SLmLoose,SLm,SLeLooseLDP,SLeLoose,SLe,LDP,GJet_ManualCleanVars,GJet_CleanVars,GJetLoose_CleanVars \
GJetLDP_ManualCleanVars,GJetLDP_CleanVars,DYm_CleanVars,DYmLDP_CleanVars,DYe_CleanVars,DYeLDP_CleanVars \
)

for ((i=0; i < ${#SAMPLES[@]}; i++)); do
  ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLES[$i]} ${SELTYPES[$i]} ${INDIR} ${OUTDIR} ${STORE}
done
