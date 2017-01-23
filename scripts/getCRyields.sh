#!/bin/bash

source batch/exportProd.sh

REGIONS=(LDP SLe SLm signal signalUnblind)
FOLDERS=(LDP SLe SLm SR SR)

for ((i=0; i < ${#REGIONS[@]}; i++)); do
	root -b -l -q 'MakeAllDCdata.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/'$RUN2PRODV'","'${REGIONS[$i]}'","'$RUN2PRODV'/datacards_'${FOLDERS[$i]}'/")'
done
