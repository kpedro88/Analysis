#!/bin/bash

source batch/exportProd.sh

SETLIST="input/input_sets_DC_data_reMiniAOD.txt"

if [[ -n "$1" ]]; then
	SETLIST=$1
fi

#REGIONS=(LDP SLe SLm signal signalUnblind)
#FOLDERS=(LDP SLe SLm SR SR)
REGIONS=(LDP SLe SLm signal)
FOLDERS=(LDP SLe SLm SR)

for ((i=0; i < ${#REGIONS[@]}; i++)); do
	root -b -l -q 'MakeAllDCdata.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/'$RUN2PRODV'","'${REGIONS[$i]}'","'$SETLIST'","'$RUN2PRODV'/datacards_'${FOLDERS[$i]}'/")'
done
