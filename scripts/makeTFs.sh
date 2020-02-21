#!/bin/bash

SETLIST=(
highCutSR,highCutCR \
lowCutSR,lowCutCR \
vlowCutSR,vlowCutCR \
v2lowCutSR,v2lowCutCR \
highSVJ2,highSVJ0 \
lowSVJ2,lowSVJ0 \
vlowSVJ2,vlowSVJ0 \
v2lowSVJ2,v2lowSVJ0 \
)

for SETS in ${SETLIST[@]}; do
	SETARR=()
	IFS="," read -a SETARR <<< "$SETS"
	HIST=input/input_svj_mt_hist_tf_fit.txt
#	if [[ "$SETS" == *"CutCR" ]]; then
#		HIST=input/input_svj_mt_hist_tf_fit_highdeta.txt
#	fi
	EXTRA="d:ratiomax[7]"
	POLY="pol0"
	if [[ "$SETS" == *"SVJ0" ]]; then
		EXTRA="d:ratiomax[0.05]"
		POLY="pol1"
	fi
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_fit.txt","'$HIST'","input/input_svj_ext_full_bkg.txt"},{"OPTION","'$EXTRA'","vs:chosensets['${SETS}']","vs:numers['${SETARR[0]}']","vs:denoms['${SETARR[1]}']","s+:printsuffix[_'${SETS}']","vs+:printformat[pdf]","s:rootfile[test/tfs_'${SETS}']"},1)'
	python applyTF.py -i test/tfs_${SETS}.root -h MTAK8_${SETARR[1]} -f ${POLY}_MTAK8_ratio__${SETARR[0]}__${SETARR[1]}
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_closure.txt","input/input_svj_mt_hist_full.txt","input/input_svj_ext_full_bkg_closure.txt"},{"OPTION","vs:chosensets['${SETS}']","vs:numers['${SETARR[0]}']","vs:denoms['${SETARR[1]}']","s+:printsuffix[_'${SETS}'_closure]","vs+:printformat[pdf]"},1)'
done
