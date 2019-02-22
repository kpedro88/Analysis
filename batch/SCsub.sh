#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_scan.txt
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
STORE=${INDIR}/scan
CHECKARGS=""
YEARS=()
DRYRUN=""

#check arguments
while getopts "ky:d" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		d) DRYRUN="echo"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for YEAR in ${YEARS[@]}; do
	SNAME=Scan${YEAR}
	source export${SNAME}.sh

	$DRYRUN ./SCtemp.sh ${JOBDIR} ${INPUT} ${SNAME} ${#SAMPLES[@]} ${INDIR} ${STORE}
done

