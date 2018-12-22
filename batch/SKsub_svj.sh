#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection_svj.txt
SELTYPE=dijetmtdetahadmf
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
CHECKARGS=""
TYPES=()
YEARS=()

#check arguments
while getopts "ky:t:" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	for YEAR in ${YEARS[@]}; do
		source exportSkim${TYPE}${YEAR}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi

		for SAMPLE in ${SAMPLES[@]}; do
			if [ "$TYPE" = data ] && [[ $SAMPLE != *"JetHT"* ]]; then continue; fi
			./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
		done
	done
done

