#!/bin/bash

runall() {
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_fit.txt","input/input_svj_mt_hist_tf_fit.txt","input/input_svj_ext_full_bkg.txt"},{"OPTION","'$EXTRA'","vs:chosensets['${SETS}']","vs:numers['${SETARR[0]}']","vs:denoms['${SETARR[1]}']","s+:printsuffix[_'${SETS}']","i:npanel[2]","b:balance_panels[1]","vs+:printformat[pdf]","s:rootfile[test/tfs_'${SETS}']"},1)'
	for SET in ${SETARR[@]}; do
		root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_fit.txt","input/input_svj_mt_hist_tf_fit.txt","input/input_svj_ext_full_bkg.txt"},{"OPTION","'$EXTRA'","vs:chosensets['${SET}']","vs:numers['${SET}']","vs:denoms['${SET}']","s+:printsuffix[_'${SET}'_res]","vs+:printformat[pdf]","s:ratiocalc[RelRes]","d:ratiomin[-0.5]","d:ratiomax[0.5]","s:rationame[residual (f-h)/f]"},1)'
	done
	python applyTF.py -i test/tfs_${SETS}.root -h MTAK8_${SETARR[1]} -f ${POLY}_MTAK8_ratio__${SETARR[0]}__${SETARR[1]}
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_closure.txt","input/input_svj_mt_hist_full.txt","input/input_svj_ext_full_bkg_closure.txt"},{"OPTION","vs:chosensets['${SETS}']","vs:numers['${SETARR[0]}']","vs:denoms['${SETARR[1]}']","s+:printsuffix[_'${SETS}'_closure]","vs+:printformat[pdf]"},1)'
	# make TF w/ extrap too
	if [ -n "$EXTRAP" ]; then
		python applyTF.py -i test/tfs_${SETS}.root -o test/applytfs_extrap_${SETS}.root -h MTAK8_${SETARR[1]} -f ${POLY}_MTAK8_ratio__${SETARR[0]}__${SETARR[1]} $EXTRAP
		# reuse ext file
		mv test/applytfs_${SETS}.root test/applytfs_${SETS}.root.bak
		mv test/applytfs_extrap_${SETS}.root test/applytfs_${SETS}.root
		root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_closure.txt","input/input_svj_mt_hist_full.txt","input/input_svj_ext_full_bkg_closure.txt"},{"OPTION","vs:chosensets['${SETS}']","vs:numers['${SETARR[0]}']","vs:denoms['${SETARR[1]}']","s+:printsuffix[_'${SETS}'_extrap_closure]","vs+:printformat[pdf]"},1)'
		mv test/applytfs_${SETS}.root test/applytfs_extrap_${SETS}.root
		mv test/applytfs_${SETS}.root.bak test/applytfs_${SETS}.root
	fi
}

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

PIDS=()

for SETS in ${SETLIST[@]}; do
	SETARR=()
	IFS="," read -a SETARR <<< "$SETS"
	HIST=input/input_svj_mt_hist_tf_fit.txt
	EXTRA="d:ratiomax[7]"
	POLY="pol0"
	EXTRAP="-e thryn_MTAK8_${SETARR[1]}"
	if [[ "$SETS" == *"SVJ0" ]]; then
		EXTRA="d:ratiomax[0.05]"
		POLY="pol1"
		EXTRAP=""
		exit 0
	fi

	runall >& log_makeTFs_${SETS}.log &
	PIDS[$SETS]=$!
done

for PID in ${PIDS[@]}; do
	wait $PID
done
