#!/bin/bash

source batch/exportProd.sh

# these arrays must all have the same length
SAMPLES=(
Summer16.SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0_RA2AnalysisTree.root \
Summer16.ZJetsToNuNu_HT-100To200_13TeV-madgraph_0_RA2AnalysisTree.root \
Spring16Fast.SMS-T5bbbbZg_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0_RA2AnalysisTree.root \
Run2016B-03Feb2017_ver2-v2.SingleElectron_0_RA2AnalysisTree.root \
#tree_T1tttt_1500_100.root \
PrivateSamples.SVJ_mZprime-3000_mDark-20_rinv-0p3_alpha-0p2_n-1000_0_RA2AnalysisTree.root \
)
DIRS=(
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
#root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/$RUN2PRODV/tree_signal \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/ProductionV2/Ntuples/ \
)
TREES=(
TreeMaker2/PreSelection \
TreeMaker2/PreSelection \
TreeMaker2/PreSelection \
TreeMaker2/PreSelection \
#tree \
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
