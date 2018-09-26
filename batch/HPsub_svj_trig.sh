#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/tree_dijetmutrig
STORE=$INDIR/histMETMT
OUTPUT=
EXTRA=
CHECKARGS=""

#check arguments
while getopts "k" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

for YEAR in 2016 2017; do
	PERIODS=()
	if [ "$YEAR" -eq 2016 ]; then
		PERIODS=(B2 C D E F G H)
	elif [ "$YEAR" -eq 2017 ]; then
		PERIODS=(B C D E F)
	fi

	INPUTS='"input/input_svj_hist_data_'${YEAR}'.txt","input/input_svj_trig'${YEAR}'.txt","input/input_svj_trig_hist2Dzoom.txt"'

	for PERIOD in ${PERIODS[@]}; do
		OUTPUT='"OPTION","string:rootfile[hist_trig_'${YEAR}${PERIOD}']"'
		# name all sets the same for each year in order to hadd
		EXTRA='"SET","hist\tdata\tSingleMuon_'${YEAR}'","\tbase\tdata\tSingleMuon_'${YEAR}${PERIOD}'\ts:filename[tree_SingleMuon_'${YEAR}${PERIOD}'.root]"'
		JOBNAME="hist_trig_"${YEAR}${PERIOD}
		./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} "$INPUTS" "$OUTPUT" "$EXTRA" "$JOBNAME"
	done
done

