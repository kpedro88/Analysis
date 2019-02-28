#!/bin/bash

source exportProd.sh

JOBDIR=jobs
JOBTYPE=trigskim
INPUT=input/input_selection_svj.txt
SELTYPE=dijetmtmutrig
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
CHECKARGS=""
YEARS=()
DRYRUN=""

#check arguments
while getopts "kdy:" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		d) DRYRUN="echo"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for YEAR in ${YEARS[@]}; do
	SNAME=SkimData${YEAR}
	source export${SNAME}.sh

	SLIST=""
	for ((i=0; i < ${#SAMPLES[@]}; i++)); do
		if [[ ${SAMPLES[$i]} == *"SingleMuon"* ]]; then
			SLIST="$SLIST,$i"
		fi
	done
	# remove first char, prepend condor stuf
	SLIST="Process in ${SLIST:1}"

	$DRYRUN ./SKtemp.sh ${JOBDIR} ${INPUT} ${SNAME} "${SLIST}" ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE} ${JOBTYPE}
done
