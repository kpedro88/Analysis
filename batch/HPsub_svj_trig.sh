#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/tree_dijetmtmutrig
STORE=$INDIR/hist
OUTPUT=
EXTRA=
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
	PERIODS=()
	BASEYEAR=$YEAR
	if [ "$YEAR" = "2016" ]; then
		PERIODS=(B C D E F G H)
	elif [ "$YEAR" = "2017" ]; then
		PERIODS=(B C D E F)
	elif [[ $YEAR = "2018"* ]]; then
		PERIODS=(A B C D)
		BASEYEAR=2018
	fi

	INPUTS='"input/input_svj_hist_data_'${YEAR}'.txt","input/input_svj_trig'${YEAR}'.txt","input/input_svj_trig_hist.txt"'

	for PERIOD in ${PERIODS[@]}; do
		OUTPUT='"OPTION","string:rootfile[hist_trig_'${YEAR}${PERIOD}']"'
		# name all sets the same for each year in order to hadd
		EXTRA='"SET","hist\tdata\tSingleMuon_'${YEAR}'","\tbase\tdata\tSingleMuon_'${YEAR}${PERIOD}'\ts:filename[tree_SingleMuon_'${BASEYEAR}${PERIOD}'.root]"'
		JOBNAME="hist_trig_"${YEAR}${PERIOD}

		echo 'KPlotDriver.C+("'"$INDIR"'",{'"$INPUTS"'},{'"$EXTRA"','"$OUTPUT"'})' > jobs/input/macro_${JOBNAME}.txt

		$DRYRUN ./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} ${JOBNAME}
	done
done

