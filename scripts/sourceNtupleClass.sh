#!/bin/bash

source batch/exportProd.sh

# these arrays must all have the same length
SAMPLES=(
Run2017B-31Mar2018-v1.SingleElectron_0_RA2AnalysisTree.root \
PrivateSamples.SVJ_2017_mZprime-3000_mDark-20_rinv-0p3_alpha-0p2_n-1000_0_RA2AnalysisTree.root \
)
DIRS=(
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/ProductionV3/2017/Ntuples/ \
)
TREES=(
TreeMaker2/PreSelection \
TreeMaker2/PreSelection \
)

HEADER="
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <vector>
#include <string>
"
