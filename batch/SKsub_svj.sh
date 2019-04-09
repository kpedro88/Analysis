#!/bin/bash

source exportProd.sh

JOBDIR=jobs
JOBTYPE=skim
INPUT=input/input_selection_svj.txt
SELTYPE=dijetmtdetahadmf
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
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

		TMPSELTYPE=$SELTYPE
		SLIST=${#SAMPLES[@]}
		if [[ $TYPE == "Data" ]]; then
			SLIST=""
			for ((i=0; i < ${#SAMPLES[@]}; i++)); do
				if [[ ${SAMPLES[$i]} == *"JetHT"* ]]; then
					SLIST="$SLIST,$i"
				fi
			done
			# remove first char, prepend condor stuff
			SLIST="Process in ${SLIST:1}"
		elif [[ $TYPE == "SVJ" ]]; then
			TMPSELTYPE=$TMPSELTYPE,dijetmtdetahadmf_JECup,dijetmtdetahadmf_JECdown,dijetmtdetahadmf_JERup,dijetmtdetahadmf_JERdown
		fi

		$DRYRUN ./SKtemp.sh ${JOBDIR} ${INPUT} ${SNAME} "${SLIST}" ${TMPSELTYPE} ${INDIR} ${OUTDIR} ${STORE} ${JOBTYPE}
	done
done

