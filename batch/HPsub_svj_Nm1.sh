#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17
STORE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Nm1/hist
CHECKARGS=""
YEARS=()
TYPE=""
DRYRUN=""
MATCH=""
GEN=

#check arguments
while getopts "kdy:t:m:g" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) TYPE="$OPTARG"
		;;
		m) MATCH="$OPTARG"
		;;
		g) GEN=gen;
		   STORE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Nm1/hist_gen
		;;
		d) DRYRUN="echo"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

if [[ "$TYPE" == "scan" ]]; then
	INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/scan
fi

for YEAR in ${YEARS[@]}; do
	INPUTS='"input/input_svj_hp_Nm1_'${YEAR}${GEN}'.txt","input/input_hp_sets_svj_'$TYPE'_'$YEAR'.txt"'

	SNAME=HistSVJ${TYPE}Unskimmed${YEAR}
	source export${SNAME}.sh

	JOBNAME="svj_Nm1_${TYPE}_${YEAR}${GEN}"
	SLIST=${#SAMPLES[@]}
	if [ -n "$MATCH" ]; then
		SLIST=""
	fi
	for ((i=0; i < ${#SAMPLES[@]}; i++)); do
		SAMPLE=${SAMPLES[$i]}
		if [ -n "$MATCH" ]; then
			if [[ "$SAMPLE" == *"$MATCH"* ]]; then
				SLIST="$SLIST,$i"
			else
				continue
			fi
		fi
		SJOBNAME=${JOBNAME}'_part'${i}
		OUTPUT='"OPTION","vstring:chosensets['${SAMPLE}']","string:rootfile['${SJOBNAME}']"'
		echo 'KPlotDriver.C+("'"$INDIR"'",{'"$INPUTS"'},{'"$OUTPUT"'})' > jobs/input/macro_${SJOBNAME}.txt
	done
	if [ -n "$MATCH" ]; then
		# remove first char, prepend condor stuff
		SLIST="Process in ${SLIST:1}"
	fi

	$DRYRUN ./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} ${JOBNAME} "${SLIST}"
done
