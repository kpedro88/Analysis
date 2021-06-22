#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_scan.txt
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/scan
CHECKARGS=""
YEARS=()
DRYRUN=""
SUFFIX=""

#check arguments
while getopts "ky:s:i:d" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		s) SUFFIX="$OPTARG"
		;;
		i) INDIR="$OPTARG"
		;;
		d) DRYRUN="echo"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for YEAR in ${YEARS[@]}; do
	SNAME=ScanSVJ${YEAR}${SUFFIX}
	source export${SNAME}.sh
	# skip nonexistent ones
	if [[ $? -ne 0 ]]; then continue; fi

	SLIST=${#SAMPLES[@]}

	$DRYRUN ./SCtemp.sh ${JOBDIR} ${INPUT} ${SNAME} "${SLIST}" ${INDIR} ${STORE}
done

