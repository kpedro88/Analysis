#!/bin/bash

export SAMPLES=(
QCD_HT-200to300_block0 \
QCD_HT-200to300_block1 \
QCD_HT-200to300_block2 \
QCD_HT-200to300_block3 \
QCD_HT-200to300_block4 \
QCD_HT-200to300_block5 \
QCD_HT-200to300_block6 \
QCD_HT-200to300_block7 \
QCD_HT-200to300_block8 \
QCD_HT-200to300_block9 \
QCD_HT-200to300_block10 \
QCD_HT-300to500_block0 \
QCD_HT-300to500_block1 \
QCD_HT-300to500_block2 \
QCD_HT-300to500_block3 \
QCD_HT-300to500_block4 \
QCD_HT-300to500_block5 \
QCD_HT-300to500_block6 \
QCD_HT-300to500_block7 \
QCD_HT-300to500_block8 \
QCD_HT-300to500_block9 \
QCD_HT-300to500_block10 \
QCD_HT-300to500_block11 \
QCD_HT-300to500_block12 \
QCD_HT-500to700_block0 \
QCD_HT-500to700_block1 \
QCD_HT-500to700_block2 \
QCD_HT-500to700_block3 \
QCD_HT-500to700_block4 \
QCD_HT-500to700_block5 \
QCD_HT-500to700_block6 \
QCD_HT-500to700_block7 \
QCD_HT-500to700_block8 \
QCD_HT-500to700_block9 \
QCD_HT-500to700_block10 \
QCD_HT-500to700_block11 \
QCD_HT-500to700_block12 \
QCD_HT-500to700_block13 \
QCD_HT-500to700_block14 \
QCD_HT-500to700_block15 \
QCD_HT-500to700_block16 \
QCD_HT-500to700_block17 \
QCD_HT-500to700_block18 \
QCD_HT-500to700_block19 \
QCD_HT-500to700_block20 \
QCD_HT-700to1000_block0 \
QCD_HT-700to1000_block1 \
QCD_HT-700to1000_block2 \
QCD_HT-700to1000_block3 \
QCD_HT-700to1000_block4 \
QCD_HT-700to1000_block5 \
QCD_HT-700to1000_block6 \
QCD_HT-700to1000_block7 \
QCD_HT-700to1000_block8 \
QCD_HT-700to1000_block9 \
QCD_HT-700to1000_block10 \
QCD_HT-1000to1500_block0 \
QCD_HT-1000to1500_block1 \
QCD_HT-1000to1500_block2 \
QCD_HT-1000to1500_block3 \
QCD_HT-1000to1500_block4 \
QCD_HT-1000to1500_block5 \
QCD_HT-1500to2000_block0 \
QCD_HT-1500to2000_block1 \
QCD_HT-1500to2000_block2 \
QCD_HT-1500to2000_block3 \
QCD_HT-1500to2000_block4 \
QCD_HT-2000toInf_block0 \
QCD_HT-2000toInf_block1 \
QCD_HT-2000toInf_block2 \
WJetsToLNu_HT-100to200_block0 \
WJetsToLNu_HT-100to200_block1 \
WJetsToLNu_HT-100to200_block2 \
WJetsToLNu_HT-100to200_block3 \
WJetsToLNu_HT-100to200_block4 \
WJetsToLNu_HT-100to200_block5 \
WJetsToLNu_HT-100to200_block6 \
WJetsToLNu_HT-200to400_block0 \
WJetsToLNu_HT-200to400_block1 \
WJetsToLNu_HT-200to400_block2 \
WJetsToLNu_HT-200to400_block3 \
WJetsToLNu_HT-200to400_block4 \
WJetsToLNu_HT-200to400_block5 \
WJetsToLNu_HT-400to600_block0 \
WJetsToLNu_HT-400to600_block1 \
WJetsToLNu_HT-400to600_block2 \
WJetsToLNu_HT-600to800_block0 \
WJetsToLNu_HT-600to800_block1 \
WJetsToLNu_HT-600to800_block2 \
WJetsToLNu_HT-600to800_block3 \
WJetsToLNu_HT-600to800_block4 \
WJetsToLNu_HT-800to1200_block0 \
WJetsToLNu_HT-800to1200_block1 \
WJetsToLNu_HT-800to1200_block2 \
WJetsToLNu_HT-1200to2500_block0 \
WJetsToLNu_HT-1200to2500_block1 \
WJetsToLNu_HT-1200to2500_block2 \
WJetsToLNu_HT-2500toInf_block0 \
WJetsToLNu_HT-2500toInf_block1 \
TTJets_block0 \
TTJets_block1 \
TTJets_block2 \
TTJets_block3 \
TTJets_SingleLeptFromT_block0 \
TTJets_SingleLeptFromT_block1 \
TTJets_SingleLeptFromT_block2 \
TTJets_SingleLeptFromT_block3 \
TTJets_SingleLeptFromT_block4 \
TTJets_SingleLeptFromT_block5 \
TTJets_SingleLeptFromT_block6 \
TTJets_SingleLeptFromT_block7 \
TTJets_SingleLeptFromT_block8 \
TTJets_SingleLeptFromT_block9 \
TTJets_SingleLeptFromT_block10 \
TTJets_SingleLeptFromT_block11 \
TTJets_SingleLeptFromT_block12 \
TTJets_SingleLeptFromT_block13 \
TTJets_SingleLeptFromT_block14 \
TTJets_SingleLeptFromT_block15 \
TTJets_SingleLeptFromTbar_block0 \
TTJets_SingleLeptFromTbar_block1 \
TTJets_SingleLeptFromTbar_block2 \
TTJets_SingleLeptFromTbar_block3 \
TTJets_SingleLeptFromTbar_block4 \
TTJets_SingleLeptFromTbar_block5 \
TTJets_SingleLeptFromTbar_block6 \
TTJets_SingleLeptFromTbar_block7 \
TTJets_SingleLeptFromTbar_block8 \
TTJets_SingleLeptFromTbar_block9 \
TTJets_SingleLeptFromTbar_block10 \
TTJets_SingleLeptFromTbar_block11 \
TTJets_SingleLeptFromTbar_block12 \
TTJets_SingleLeptFromTbar_block13 \
TTJets_SingleLeptFromTbar_block14 \
TTJets_SingleLeptFromTbar_block15 \
TTJets_DiLept_block0 \
TTJets_DiLept_block1 \
TTJets_DiLept_block2 \
TTJets_DiLept_block3 \
TTJets_DiLept_block4 \
TTJets_DiLept_block5 \
TTJets_DiLept_block6 \
TTJets_DiLept_block7 \
TTJets_DiLept_block8 \
TTJets_SingleLeptFromT_genMET-150_block0 \
TTJets_SingleLeptFromT_genMET-150_block1 \
TTJets_SingleLeptFromT_genMET-150_block2 \
TTJets_SingleLeptFromT_genMET-150_block3 \
TTJets_SingleLeptFromT_genMET-150_block4 \
TTJets_SingleLeptFromT_genMET-150_block5 \
TTJets_SingleLeptFromT_genMET-150_block6 \
TTJets_SingleLeptFromT_genMET-150_block7 \
TTJets_SingleLeptFromTbar_genMET-150_block0 \
TTJets_SingleLeptFromTbar_genMET-150_block1 \
TTJets_SingleLeptFromTbar_genMET-150_block2 \
TTJets_SingleLeptFromTbar_genMET-150_block3 \
TTJets_SingleLeptFromTbar_genMET-150_block4 \
TTJets_SingleLeptFromTbar_genMET-150_block5 \
TTJets_SingleLeptFromTbar_genMET-150_block6 \
TTJets_DiLept_genMET-150_block0 \
TTJets_DiLept_genMET-150_block1 \
TTJets_DiLept_genMET-150_block2 \
TTJets_DiLept_genMET-150_block3 \
TTJets_DiLept_genMET-150_block4 \
TTJets_HT-600to800_block0 \
TTJets_HT-600to800_block1 \
TTJets_HT-600to800_block2 \
TTJets_HT-600to800_block3 \
TTJets_HT-600to800_block4 \
TTJets_HT-600to800_block5 \
TTJets_HT-600to800_block6 \
TTJets_HT-800to1200_block0 \
TTJets_HT-800to1200_block1 \
TTJets_HT-800to1200_block2 \
TTJets_HT-800to1200_block3 \
TTJets_HT-800to1200_block4 \
TTJets_HT-1200to2500_block0 \
TTJets_HT-1200to2500_block1 \
TTJets_HT-2500toInf \
DYJetsToLL_M-50_HT-100to200_block0 \
DYJetsToLL_M-50_HT-100to200_block1 \
DYJetsToLL_M-50_HT-100to200_block2 \
DYJetsToLL_M-50_HT-100to200_block3 \
DYJetsToLL_M-50_HT-200to400_block0 \
DYJetsToLL_M-50_HT-200to400_block1 \
DYJetsToLL_M-50_HT-200to400_block2 \
DYJetsToLL_M-50_HT-200to400_block3 \
DYJetsToLL_M-50_HT-400to600_block0 \
DYJetsToLL_M-50_HT-400to600_block1 \
DYJetsToLL_M-50_HT-400to600_block2 \
DYJetsToLL_M-50_HT-400to600_block3 \
DYJetsToLL_M-50_HT-600to800_block0 \
DYJetsToLL_M-50_HT-600to800_block1 \
DYJetsToLL_M-50_HT-600to800_block2 \
DYJetsToLL_M-50_HT-600to800_block3 \
DYJetsToLL_M-50_HT-600to800_block4 \
DYJetsToLL_M-50_HT-600to800_block5 \
DYJetsToLL_M-50_HT-600to800_block6 \
DYJetsToLL_M-50_HT-600to800_block7 \
DYJetsToLL_M-50_HT-800to1200_block0 \
DYJetsToLL_M-50_HT-800to1200_block1 \
DYJetsToLL_M-50_HT-1200to2500 \
DYJetsToLL_M-50_HT-2500toInf \
DYJetsToLL_M-50_block0 \
DYJetsToLL_M-50_block1 \
DYJetsToLL_M-50_block2 \
DYJetsToLL_M-50_block3 \
DYJetsToLL_M-50_block4 \
DYJetsToLL_M-50_block5 \
DYJetsToLL_M-50_block6 \
DYJetsToLL_M-50_block7 \
DYJetsToLL_M-50_block8 \
DYJetsToLL_M-50_block9 \
ZJetsToNuNu_HT-100to200_block0 \
ZJetsToNuNu_HT-100to200_block1 \
ZJetsToNuNu_HT-100to200_block2 \
ZJetsToNuNu_HT-100to200_block3 \
ZJetsToNuNu_HT-100to200_block4 \
ZJetsToNuNu_HT-100to200_block5 \
ZJetsToNuNu_HT-200to400_block0 \
ZJetsToNuNu_HT-200to400_block1 \
ZJetsToNuNu_HT-200to400_block2 \
ZJetsToNuNu_HT-200to400_block3 \
ZJetsToNuNu_HT-200to400_block4 \
ZJetsToNuNu_HT-200to400_block5 \
ZJetsToNuNu_HT-200to400_block6 \
ZJetsToNuNu_HT-400to600_block0 \
ZJetsToNuNu_HT-400to600_block1 \
ZJetsToNuNu_HT-400to600_block2 \
ZJetsToNuNu_HT-600to800_block0 \
ZJetsToNuNu_HT-600to800_block1 \
ZJetsToNuNu_HT-600to800_block2 \
ZJetsToNuNu_HT-800to1200 \
ZJetsToNuNu_HT-1200to2500 \
ZJetsToNuNu_HT-2500toInf \
GJets_HT-100to200_block0 \
GJets_HT-100to200_block1 \
GJets_HT-100to200_block2 \
GJets_HT-200to400_block0 \
GJets_HT-200to400_block1 \
GJets_HT-200to400_block2 \
GJets_HT-200to400_block3 \
GJets_HT-200to400_block4 \
GJets_HT-200to400_block5 \
GJets_HT-200to400_block6 \
GJets_HT-200to400_block7 \
GJets_HT-400to600_block0 \
GJets_HT-400to600_block1 \
GJets_HT-600toInf_block0 \
GJets_HT-600toInf_block1 \
GJets_HT-600toInf_block2 \
GJets_DR-0p4_HT-100to200_block0 \
GJets_DR-0p4_HT-100to200_block1 \
GJets_DR-0p4_HT-100to200_block2 \
GJets_DR-0p4_HT-100to200_block3 \
GJets_DR-0p4_HT-100to200_block4 \
GJets_DR-0p4_HT-200to400_block0 \
GJets_DR-0p4_HT-200to400_block1 \
GJets_DR-0p4_HT-200to400_block2 \
GJets_DR-0p4_HT-200to400_block3 \
GJets_DR-0p4_HT-200to400_block4 \
GJets_DR-0p4_HT-200to400_block5 \
GJets_DR-0p4_HT-200to400_block6 \
GJets_DR-0p4_HT-200to400_block7 \
GJets_DR-0p4_HT-200to400_block8 \
GJets_DR-0p4_HT-200to400_block9 \
GJets_DR-0p4_HT-200to400_block10 \
GJets_DR-0p4_HT-200to400_block11 \
GJets_DR-0p4_HT-200to400_block12 \
GJets_DR-0p4_HT-200to400_block13 \
GJets_DR-0p4_HT-200to400_block14 \
GJets_DR-0p4_HT-200to400_block15 \
GJets_DR-0p4_HT-200to400_block16 \
GJets_DR-0p4_HT-200to400_block17 \
GJets_DR-0p4_HT-400to600_block0 \
GJets_DR-0p4_HT-400to600_block1 \
GJets_DR-0p4_HT-400to600_block2 \
GJets_DR-0p4_HT-400to600_block3 \
GJets_DR-0p4_HT-400to600_block4 \
GJets_DR-0p4_HT-600toInf_block0 \
GJets_DR-0p4_HT-600toInf_block1 \
GJets_DR-0p4_HT-600toInf_block2 \
GJets_DR-0p4_HT-600toInf_block3 \
GJets_DR-0p4_HT-600toInf_block4 \
GJets_DR-0p4_HT-600toInf_block5 \
GJets_DR-0p4_HT-600toInf_block6 \
GJets_DR-0p4_HT-600toInf_block7 \
ST_s-channel \
ST_t-channel_top_block0 \
ST_t-channel_top_block1 \
ST_t-channel_top_block2 \
ST_t-channel_top_block3 \
ST_t-channel_top_block4 \
ST_t-channel_top_block5 \
ST_t-channel_top_block6 \
ST_t-channel_top_block7 \
ST_t-channel_top_block8 \
ST_t-channel_top_block9 \
ST_t-channel_top_block10 \
ST_t-channel_top_block11 \
ST_t-channel_top_block12 \
ST_t-channel_top_block13 \
ST_t-channel_top_block14 \
ST_t-channel_antitop_block0 \
ST_t-channel_antitop_block1 \
ST_t-channel_antitop_block2 \
ST_t-channel_antitop_block3 \
ST_t-channel_antitop_block4 \
ST_t-channel_antitop_block5 \
ST_t-channel_antitop_block6 \
ST_t-channel_antitop_block7 \
ST_t-channel_antitop_block8 \
ST_tW_top_block0 \
ST_tW_top_block1 \
ST_tW_top_block2 \
ST_tW_top_block3 \
ST_tW_top_block4 \
ST_tW_antitop_block0 \
ST_tW_antitop_block1 \
ST_tW_antitop_block2 \
ST_tW_antitop_block3 \
WGJets_MonoPhoton_PtG-40to130_block0 \
WGJets_MonoPhoton_PtG-40to130_block1 \
WGJets_MonoPhoton_PtG-130 \
WWTo2L2Nu \
WWTo1L1Nu2Q_block0 \
WWTo1L1Nu2Q_block1 \
WZTo1L1Nu2Q_block0 \
WZTo1L1Nu2Q_block1 \
WZTo1L1Nu2Q_block2 \
WZTo1L1Nu2Q_block3 \
WZTo1L1Nu2Q_block4 \
WZTo1L1Nu2Q_block5 \
WZTo1L1Nu2Q_block6 \
WZTo1L1Nu2Q_block7 \
WZTo1L1Nu2Q_block8 \
WZTo1L1Nu2Q_block9 \
WZTo1L1Nu2Q_block10 \
WZTo1L3Nu \
ZGTo2NuG \
ZZTo2Q2Nu_block0 \
ZZTo2Q2Nu_block1 \
ZZTo2Q2Nu_block2 \
ZZTo2Q2Nu_block3 \
ZZTo2Q2Nu_block4 \
ZZTo2Q2Nu_block5 \
ZZTo2Q2Nu_block6 \
ZZTo2Q2Nu_block7 \
ZZTo2Q2Nu_block8 \
ZZTo2Q2Nu_block9 \
ZZTo2Q2Nu_block10 \
ZZTo2L2Q_block0 \
ZZTo2L2Q_block1 \
ZZTo2L2Q_block2 \
ZZTo2L2Q_block3 \
ZZTo2L2Q_block4 \
ZZTo2L2Q_block5 \
ZZTo2L2Q_block6 \
WWZ \
ZZZ \
WZZ \
TTWJetsToQQ \
TTWJetsToLNu_block0 \
TTWJetsToLNu_block1 \
TTZToQQ \
TTZToLLNuNu \
TTGJets_block0 \
TTGJets_block1 \
TTGJets_block2 \
TTGJets_block3 \
TTGJets_block4 \
TTGJets_block5 \
TTGJets_block6 \
TTTT \
)
