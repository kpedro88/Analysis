#!/bin/bash

KEEPTAR=$1
JOBDIR=batch
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

for SAMPLE in T1bbbb_1000_900 T1bbbb_1500_100 T1qqqq_1000_800 T1qqqq_1400_100 T1tttt_1200_800 T1tttt_1500_100 T2bb_600_580 T2bb_900_100 T2qq_600_550 T2qq_1200_100 DYJetsToLL_M-50_HT-400to600 DYJetsToLL_M-50_HT-600toInf GJets_HT-400to600 GJets_HT-600toInf QCD_HT-500to1000 QCD_HT-500to1000_ext1 QCD_HT-1000toInf QCD_HT-1000toInf_ext1 TTJets_part1 TTJets_part2 TTJets_part3 TTJets_part4 TTJets_part5 WJetsToLNu_HT-400to600 WJetsToLNu_HT-600toInf_part1 WJetsToLNu_HT-600toInf_part2 WJetsToLNu_HT-600toInf_part3 ZJetsToNuNu_HT-400to600 ZJetsToNuNu_HT-600toInf
  do
    ./SKtemp.sh ${JOBDIR} input/input_selection.txt ${SAMPLE} signal,LDP,SLm,SLe,GJet_CleanVars,DYm_CleanVars,DYe_CleanVars root://cmseos.fnal.gov//store/user/awhitbe1/PHYS14productionV15 tree ${STORE}
  done

for SAMPLE in DoubleEG DoubleMuon EGamma HTMHT MET SingleElectron SingleMu SingleMuon SinglePhoton
  do
    ./SKtemp.sh ${JOBDIR} input/input_selection_data.txt ${SAMPLE} signal,LDP,SLm,SLe,GJet_CleanVars,DYm_CleanVars,DYe_CleanVars root://cmseos.fnal.gov//store/user/awhitbe1/Run2015B-PromptReco_July17 tree ${STORE}
  done
