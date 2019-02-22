#!/bin/bash

source exportProd.sh

JOBDIR=jobs
JOBTYPE=trainskim
INPUT=input/input_selection_svj_train.txt
SELTYPE=dijetmtdetahadmf-train-flatsig
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/tree_dijetmtdetahadmf
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
CHECKARGS=""
TYPES=()
DRYRUN=""

#check arguments
while getopts "kt:d" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
		d) DRYRUN="echo"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	SNAME=Skim${TYPE}2017
	source export${SNAME}.sh
	# skip nonexistent ones
	if [[ $? -ne 0 ]]; then continue; fi
	
	$DRYRUN ./SKtemp.sh ${JOBDIR} ${INPUT} ${SNAME} ${#SAMPLES[@]} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE} ${JOBTYPE}
done
