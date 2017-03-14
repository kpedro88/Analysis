#!/bin/bash

HISTOS=(
njets \
nbjets \
ht \
mht \
)
DIRS=(
signalMinusNJet \
signal \
signalMinusHT \
signalMinusMHT \
)

SIGNALS=(
gg_uncompressed \
gg_compressed \
qq_uncompressed \
qq_compressed \
)

for ((i=0; i < ${#HISTOS[@]}; i++)); do
	for SIGNAL in ${SIGNALS[@]}; do
		root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/scan/supplementary/tree_'${DIRS[$i]}'",{"input/input_comp_fast_nMinus1.txt","input/input_histo_'${HISTOS[$i]}'.txt","input/input_fast_'${SIGNAL}'.txt"},{"OPTION","string:rootfile['${DIRS[$i]}_${SIGNAL}']"},1)'
	done
done

for k in *.eps; do
	epstopdf $k
	rm $k
done
