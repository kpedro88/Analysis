#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection_qcd.txt
SELTYPE=genmet,met
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV14
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/qcd/
CHECKARGS=""

#check arguments
while getopts "k" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

SAMPLES=(
QCD_Pt_80to120_block0 \
QCD_Pt_80to120_block1 \
QCD_Pt_80to120_block2 \
QCD_Pt_120to170_block0 \
QCD_Pt_120to170_block1 \
QCD_Pt_120to170_block2 \
QCD_Pt_170to300_block0 \
QCD_Pt_170to300_block1 \
QCD_Pt_170to300_block2 \
QCD_Pt_170to300_block3 \
QCD_Pt_300to470_block0 \
QCD_Pt_300to470_block1 \
QCD_Pt_300to470_block2 \
QCD_Pt_300to470_block3 \
QCD_Pt_300to470_block4 \
QCD_Pt_300to470_block5 \
QCD_Pt_300to470_block6 \
QCD_Pt_470to600_block0 \
QCD_Pt_470to600_block1 \
QCD_Pt_600to800_block0 \
QCD_Pt_600to800_block1 \
QCD_Pt_600to800_block2 \
QCD_Pt_600to800_block3 \
QCD_Pt_600to800_block4 \
QCD_Pt_800to1000_block0 \
QCD_Pt_800to1000_block1 \
QCD_Pt_800to1000_block2 \
QCD_Pt_800to1000_block3 \
QCD_Pt_800to1000_block4 \
QCD_Pt_800to1000_block5 \
QCD_Pt_1000to1400_block0 \
QCD_Pt_1000to1400_block1 \
QCD_Pt_1000to1400_block2 \
QCD_Pt_1000to1400_block3 \
QCD_Pt_1400to1800 \
QCD_Pt_1800to2400 \
QCD_Pt_2400to3200 \
QCD_Pt_3200toInf \
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
)

for SAMPLE in ${SAMPLES[@]}
  do
    ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
  done
