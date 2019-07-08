#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/tree_dijetmtmutrig
OUTDIR=hist
STORE=$INDIR/$OUTDIR
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
	INPUTS='"input/input_svj_hist_data_'${YEAR}'.txt","input/input_svj_trig'${YEAR}'.txt","input/input_svj_trig_hist.txt","input/input_svj_hp_sets_trig.txt"'

	SNAME=HistTrig${YEAR}
	source export${SNAME}.sh

	# skip nonexistent ones
	if [[ $? -ne 0 ]]; then continue; fi

	JOBNAME=${OUTDIR}'_svj_trig'${YEAR}
	SLIST=${#SAMPLES[@]}
	for ((i=0; i < ${#SAMPLES[@]}; i++)); do
		SAMPLE=${SAMPLES[$i]}
		SJOBNAME=${JOBNAME}'_part'${i}
		OUTPUT='"OPTION","vstring:chosensets['${SAMPLE}']","string:rootfile['${SJOBNAME}']"'
		echo 'KPlotDriver.C+("'"$INDIR"'",{'"$INPUTS"'},{'"$OUTPUT"'})' > jobs/input/macro_${SJOBNAME}.txt
	done
		
	$DRYRUN ./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} ${JOBNAME} "${SLIST}"
done

