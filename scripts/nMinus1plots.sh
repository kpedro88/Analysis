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

for ((i=0; i < ${#HISTOS[@]}; i++)); do
	for j in gg_uncompressed gg_compressed qq_uncompressed qq_compressed; do
		root -b -l -q 'KPlotDriverDCsyst.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV9/scan/supplementary/tree_'${DIRS[$i]}'","input/input_comp_fast_nMinus1.txt","input/input_histo_'${HISTOS[$i]}'.txt,input/input_fast_'${j}'.txt","'${DIRS[$i]}_${j}'","",0,1)'
	done
done

for k in *.eps; do
	epstopdf $k
	rm $k
done
