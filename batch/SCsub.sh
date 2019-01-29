#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_scan.txt
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
STORE=${INDIR}/scan
CHECKARGS=""
YEARS=()

#check arguments
while getopts "ky:" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for YEAR in ${YEARS[@]}; do
	source exportScan${YEAR}.sh

	for SAMPLE in ${SAMPLES[@]}; do
		./SCtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${INDIR} ${STORE}
	done
done

