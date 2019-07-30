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

YEARS=(
2016 \
2017 \
2018 \
2018HEM \
)

ROOTFILE=""
PLOTFILE=""

#check arguments
while getopts "rp" opt; do
	case "$opt" in
		r) ROOTFILE=true
		;;
		p) PLOTFILE=true
		;;
	esac
done

if [ -n "$ROOTFILE" ]; then
	OUTNAMES=""
	for ((j=0; j < ${#SIGNALS[@]}; j++)); do
		for ((i=0; i < ${#HISTOS[@]}; i++)); do
			for YEAR in ${YEARS[@]}; do
				OUTNAME='test/supp_'${HISTOS[$i]}'_'${SIGNALS[$j]}'_'${YEAR}
				OUTNAMES="${OUTNAME}.root $OUTNAMES"
				root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17/scan/supplementary/tree_'${DIRS[$i]}'",{"input/input_comp_fast_nMinus1.txt","input/input_comp_fast_nMinus1_'${YEAR}'.txt","input/input_histo_'${HISTOS[$i]}'.txt","input/input_fast_'${SIGNALS[$j]}'_'${YEAR}'.txt"},{"OPTION","string:rootfile['${OUTNAME}]'"},0)'
			done
		done
	done
	# hadd together for easier plotting
	hadd -f test/supp_histos.root $OUTNAMES
fi

if [ -n "$PLOTFILE" ]; then
	for ((j=0; j < ${#SIGNALS[@]}; j++)); do
		for ((i=0; i < ${#HISTOS[@]}; i++)); do
			root -b -l -q 'KPlotDriver.C+(".",{"input/input_comp_fast_nMinus1.txt","input/input_histo_'${HISTOS[$i]}'.txt","input/input_fast_'${SIGNALS[$j]}'_ext.txt"},{"OPTION","string:rootfile[CMS-SUS-19-006_Figure-aux_'${FIGNUMS[$j]}-${FIGLETS[$i]}']"},1)'
		done
	done
fi
