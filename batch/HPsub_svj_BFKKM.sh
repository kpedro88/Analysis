#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17
CHECKARGS=""
YEARS=()
DRYRUN=""
STYPE=""

#check arguments
while getopts "kdy:t:" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) STYPE="$OPTARG"
		;;
		d) DRYRUN="echo"
		;;
	esac
done

STORE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/bfkkm/${STYPE}hist

if [ "$STYPE" != q ] && [ "$STYPE" != s ] && [ "$STYPE" != c ]; then
	echo "Unknown sample stype $STYPE"
	exit 1
fi

./SKcheck.sh ${CHECKARGS}

for YEAR in ${YEARS[@]}; do
	INPUTS='"input/input_svj_discr_unskimmed.txt","input/input_svj_rocs_bothjet_E1.txt"'

	SNAME=HistBFKKM${STYPE}${YEAR}
	source export${SNAME}.sh

	JOBNAME='svj_bfkkm'${STYPE}${YEAR}'_discr'
	SLIST=${#SAMPLES[@]}
	for ((i=0; i < ${#SAMPLES[@]}; i++)); do
		SAMPLE=${SAMPLES[$i]}
		SJOBNAME=${JOBNAME}'_part'${i}
		OUTPUT='"OPTION","vstring:chosensets['${SAMPLE}']","string:rootfile['${SJOBNAME}']"'
		echo 'KPlotDriver.C+("'"$INDIR"'",{'"$INPUTS"'},{'"$OUTPUT"'})' > jobs/input/macro_${SJOBNAME}.txt
	done

	$DRYRUN ./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} ${JOBNAME} "${SLIST}"
done
