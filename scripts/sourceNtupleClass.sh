#!/bin/bash

source batch/exportProd.sh

# these arrays must all have the same length
SAMPLES=(
genmassanalysis_mZprime-3000_mDark-20_rinv-0.3_alpha-0.2_n-10000_part-1.root \
)
DIRS=(
root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/$RUN2PRODV \
)
TREES=(
GenMassAnalyzer/tree \
)

HEADER="
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <vector>
#include <string>
"
