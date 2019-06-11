#!/bin/bash

source exportProd.sh

JOBDIR=jobs
JOBTYPE=trainskim
INPUT=input/input_selection_svj_train.txt
SELTYPE=dijetmtdetahadloosemf-train-flatsig
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/tree_dijetmtdetahadloosemf
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
CHECKARGS=""
TYPES=()
YEARS=()
DRYRUN=""

#check arguments
while getopts "ky:t:d" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
		d) DRYRUN="echo"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	for YEAR in ${YEARS[@]}; do
		SNAME=Skim${TYPE}${YEAR}
		source export${SNAME}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi
	
		$DRYRUN ./SKtemp.sh ${JOBDIR} ${INPUT} ${SNAME} ${#SAMPLES[@]} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE} ${JOBTYPE}
	done
done
