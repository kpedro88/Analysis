#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection_svj_train.txt
SELTYPE=dijetmtdetahadmf-train-flatsig
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/tree_dijetmtdetahadmf
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
CHECKARGS=""
TYPES=()

#check arguments
while getopts "kt:" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	if [ "$TYPE" = SVJ ]; then
		source exportSkim${TYPE}2017.sh
	else
		SAMPLES=(
QCD_Pt_1000to1400_MC2017 \
QCD_Pt_120to170_MC2017 \
QCD_Pt_1400to1800_MC2017 \
QCD_Pt_170to300_MC2017 \
QCD_Pt_1800to2400_MC2017 \
QCD_Pt_2400to3200_MC2017 \
QCD_Pt_300to470_MC2017 \
QCD_Pt_3200toInf_MC2017 \
QCD_Pt_470to600_MC2017 \
QCD_Pt_600to800_MC2017 \
QCD_Pt_800to1000_MC2017 \
QCD_Pt_80to120_MC2017 \
		)
	fi
	
	for SAMPLE in ${SAMPLES[@]}; do
		./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
	done
done
