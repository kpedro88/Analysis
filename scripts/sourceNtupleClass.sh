#!/bin/bash

source batch/exportProd.sh

# these arrays must all have the same length
SAMPLES=(
Autumn18.TTJets_TuneCP5_13TeV-madgraphMLM-pythia8_0_RA2AnalysisTree.root \
Fall17.TTJets_TuneCP5_13TeV-madgraphMLM-pythia8_0_RA2AnalysisTree.root \
Run2018B-17Sep2018-v1.SingleMuon_0_RA2AnalysisTree.root \
Run2018B-17Sep2018-v1.EGamma_0_RA2AnalysisTree.root \
tree_T1tttt_2000_100_MC2017.root \
tree_SVJ_mZprime-3000_mDark-20_rinv-0.3_alpha-peak_MC2017.root \
)
DIRS=(
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/$RUN2PRODV \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/$RUN2PRODV/tree_signal \
root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/$RUN2PRODV/Skims/tree_dijetmtdetahadmf \
)
TREES=(
TreeMaker2/PreSelection \
TreeMaker2/PreSelection \
TreeMaker2/PreSelection \
TreeMaker2/PreSelection \
tree \
tree \
)

HEADER="
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <vector>
#include <string>
"
