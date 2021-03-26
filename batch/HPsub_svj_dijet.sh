#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/scan
CHECKARGS=""
DRYRUN=""

#check arguments
while getopts "kdy:t:" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		d) DRYRUN="echo"
		;;
	esac
done

STORE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/dijet/

./SKcheck.sh ${CHECKARGS}

INPUTS='"input/input_svj_cmsdijet.txt"'

SNAME=DCSVJScanRun2
source export${SNAME}.sh

JOBNAME='svj_dijet'
SLIST=${#SAMPLES[@]}
for ((i=0; i < ${#SAMPLES[@]}; i++)); do
	SAMPLE=${SAMPLES[$i]}
	SJOBNAME=${JOBNAME}'_part'${i}
	OUTPUT='"OPTION","vstring:chosensets['${SAMPLE}']","string:rootfile['${SJOBNAME}']"'
	echo 'KPlotDriver.C+("'"$INDIR"'",{'"$INPUTS"'},{'"$OUTPUT"'})' > jobs/input/macro_${SJOBNAME}.txt
done

$DRYRUN ./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} ${JOBNAME} "${SLIST}"
