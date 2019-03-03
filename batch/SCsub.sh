#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_scan.txt
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
STORE=${INDIR}/scan
CHECKARGS=""
YEARS=()
TYPES=()
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

for YEAR in ${YEARS[@]}; do
	SNAME=Scan${YEAR}
	source export${SNAME}.sh
	# skip nonexistent ones
	if [[ $? -ne 0 ]]; then continue; fi

	SLIST=${#SAMPLES[@]}
	if [[ ${#TYPES[@]} -gt 0 ]]; then
		SLIST=""
		for TYPE in ${TYPES[@]}; do
			for ((i=0; i < ${#SAMPLES[@]}; i++)); do
				if [[ ${SAMPLES[$i]} == "$TYPE"* ]]; then
					SLIST="$SLIST,$i"
				fi
			done
		done
		if [ -n "$DRYRUN" ]; then
			NUMJOBS=$(echo $SLIST | tr -cd , | wc -c)
			echo "Number of jobs: $NUMJOBS"
		fi
		# remove first char, prepend condor stuff
		SLIST="Process in ${SLIST:1}"
	fi

	$DRYRUN ./SCtemp.sh ${JOBDIR} ${INPUT} ${SNAME} "${SLIST}" ${INDIR} ${STORE}
done

