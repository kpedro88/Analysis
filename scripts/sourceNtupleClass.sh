#!/bin/bash

source batch/exportProd.sh

# these arrays must all have the same length
SAMPLES=(
RunIIFall17MiniAODv2.TTJets_TuneCP5_13TeV-madgraphMLM-pythia8_0_RA2AnalysisTree.root \
PrivateSamples.SVJ_2017_mZprime-3000_mDark-20_rinv-0p3_alpha-0p2_n-1000_0_RA2AnalysisTree.root \
)
DIRS=(
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
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
