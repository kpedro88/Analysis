#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection_cmsdas2016.txt
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV5
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016
CHECKARGS=""

#check arguments
while getopts "k" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

SELTYPE=""
SAMPLES=()

for i in $(seq 1 6); do
  case "$i" in
  1)
    SELTYPE=signal
    SAMPLES=(
    ZJetsToNuNu_HT-100to200 \
    ZJetsToNuNu_HT-200to400 \
    ZJetsToNuNu_HT-400to600 \
    ZJetsToNuNu_HT-600toInf \
    T1tttt_1200_800 \
    T1tttt_1500_100 \
    T1tttt_600_100_fast \
    T1tttt_700_100_fast \
    T1tttt_800_100_fast \
    T1tttt_900_100_fast \
    T1tttt_1000_100_fast \
    T1tttt_1050_100_fast \
    T1tttt_1100_100_fast \
    T1tttt_1150_100_fast \
    T1tttt_1200_100_fast \
    T1tttt_1250_100_fast \
    T1tttt_1300_100_fast \
    T1tttt_1350_100_fast \
    T1tttt_1400_100_fast \
    T1tttt_1450_100_fast \
    T1tttt_1500_100_fast \
    T1tttt_1550_100_fast \
    T1tttt_1600_100_fast \
    T1tttt_1650_100_fast \
    T1tttt_1700_100_fast \
    T1tttt_1750_100_fast \
    T1tttt_1800_100_fast \
    T1tttt_1850_100_fast \
    T1tttt_1900_100_fast \
    T1tttt_1950_100_fast \
    )
  ;;
  2)
    SELTYPE=signal,SL
    SAMPLES=(
    WJetsToLNu_HT-2500toInf \
    WJetsToLNu_HT-1200to2500 \
    WJetsToLNu_HT-800to1200 \
    WJetsToLNu_HT-600to800 \
    WJetsToLNu_HT-400to600 \
    WJetsToLNu_HT-200to400 \
    WJetsToLNu_HT-100to200 \
    ST_s-channel \
    ST_t-channel_top \
    ST_t-channel_antitop \
    ST_tW_top \
    ST_tW_antitop \
    HTMHT_re2015C \
    HTMHT_re2015D \
    HTMHT_2015Db \
    )
  ;;
  3)
    SELTYPE=signal,SL,DYm_CleanVarsDY
    SAMPLES=(
    TTJets \
    TTJets_SingleLeptFromT \
    TTJets_SingleLeptFromT_ext1 \
    TTJets_SingleLeptFromTbar \
    TTJets_SingleLeptFromTbar_ext1 \
    TTJets_DiLept \
    TTJets_DiLept_ext1 \
    TTJets_HT-600to800 \
    TTJets_HT-800to1200 \
    TTJets_HT-1200to2500 \
    TTJets_HT-2500toInf \
    )
  ;;
  4)
    SELTYPE=signal,GJet_CleanVarsGJ
    SAMPLE=(
    QCD_HT-200to300 \
    QCD_HT-300to500 \
    QCD_HT-500to700 \
    QCD_HT-700to1000 \
    QCD_HT-1000to1500 \
    QCD_HT-1500to2000 \
    QCD_HT-2000toInf \
    )
  ;;
  5)
    SELTYPE=GJet_CleanVarsGJ
    SAMPLES=(
    GJets_HT-100to200 \
    GJets_HT-200to400 \
    GJets_HT-400to600 \
    GJets_HT-600toInf \
    SinglePhoton_re2015C \
    SinglePhoton_re2015D \
    SinglePhoton_2015Db \
    )
  ;;
  6)
    SELTYPE=DYm_CleanVarsDY
    SAMPLES=(
    DYJetsToLL_M-50_HT-100to200 \
    DYJetsToLL_M-50_HT-200to400 \
    DYJetsToLL_M-50_HT-400to600 \
    DYJetsToLL_M-50_HT-600toInf \
    SingleMuon_re2015C \
    SingleMuon_re2015D \
    SingleMuon_2015Db \
    )
  ;;
  esac
  
  for SAMPLE in ${SAMPLES[@]}
    do
	  INDIR2=${INDIR}
      if (echo ${SAMPLE} | fgrep -q "fast"); then
		INDIR2=${INDIR}/scan
	  fi
      ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR2} ${OUTDIR} ${STORE}
    done
done
