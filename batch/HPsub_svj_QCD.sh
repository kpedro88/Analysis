#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17
STORE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/qcd/hist3
CHECKARGS=""
YEARS=()
DRYRUN=""

#check arguments
while getopts "kdy:" opt; do
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
	INPUTS='"input/input_svj_hist.txt","input/input_svj_qcd_met_weight_sel_filter.txt","input/input_svj_qcd_met_weight_hist.txt","input/input_sets_hp_qcd_sep.txt"'

	SNAME=HistQCD${YEAR}
	source export${SNAME}.sh

	JOBNAME='svj_qcd'${YEAR}'_met'
	SLIST=${#SAMPLES[@]}
	for ((i=0; i < ${#SAMPLES[@]}; i++)); do
		SAMPLE=${SAMPLES[$i]}
		SJOBNAME=${JOBNAME}'_part'${i}
		OUTPUT='"OPTION","vstring:chosensets['${SAMPLE}']","string:rootfile['${SJOBNAME}']"'
		echo 'KPlotDriver.C+("'"$INDIR"'",{'"$INPUTS"'},{'"$OUTPUT"'})' > jobs/input/macro_${SJOBNAME}.txt
	done

	$DRYRUN ./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} ${JOBNAME} "${SLIST}"
done
