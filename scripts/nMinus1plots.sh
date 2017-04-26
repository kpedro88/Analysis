#!/bin/bash

HISTOS=(
ht \
mht \
njets \
nbjets \
)
DIRS=(
signalMinusHT \
signalMinusMHT \
signalMinusNJet \
signal \
)
FIGLETS=(
a \
b \
c \
d \
)

SIGNALS=(
gg_uncompressed \
gg_compressed \
qq_uncompressed \
qq_compressed \
)
FIGNUMS=(
001 \
002 \
003 \
004 \
)

for ((j=0; j < ${#SIGNALS[@]}; j++)); do
	for ((i=0; i < ${#HISTOS[@]}; i++)); do
		root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/scan/supplementary/tree_'${DIRS[$i]}'",{"input/input_comp_fast_nMinus1.txt","input/input_histo_'${HISTOS[$i]}'.txt","input/input_fast_'${SIGNALS[$j]}'.txt"},{"OPTION","string:rootfile[CMS-SUS-16-033_Figure-aux_'${FIGNUMS[$j]}-${FIGLETS[$i]}']"},1)'
	done
done

for k in *.eps; do
	epstopdf $k
	rm $k
done
