#!/bin/bash

runall() {
	# make plots and output root files separately (former should just have CRs, latter should have all histos)
	# also make similar TF plot for SRs to compare
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_fit.txt","input/input_svj_mt_hist_tf_fit.txt","'${SETFILE}'"},{"OPTION","'$EXTRA'","vs:chosensets['${SETARR[0]},${SETARR[1]}']","vs:numers['${SETARR[0]}']","vs:denoms['${SETARR[1]}']","s+:printsuffix[_'${SETARR[0]},${SETARR[1]}']","i:npanel[2]","b:balance_panels[1]","vs+:printformat[pdf]"},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_fit.txt","input/input_svj_mt_hist_tf_fit.txt","'${SETFILE}'"},{"OPTION","'$EXTRA'","vs:chosensets['${SETARR[3]},${SETARR[2]}']","vs:numers['${SETARR[3]}']","vs:denoms['${SETARR[2]}']","s+:printsuffix[_'${SETARR[3]},${SETARR[2]}']","i:npanel[2]","b:balance_panels[1]","vs+:printformat[pdf]"},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_fit.txt","input/input_svj_mt_hist_tf_fit.txt","'${SETFILE}'"},{"OPTION","'$EXTRA'","vs:chosensets['${SETS}']","vs:numers['${SETARR[0]}']","vs:denoms['${SETARR[1]}']","s+:printsuffix[_'${SETS}']","i:npanel[2]","b:balance_panels[1]","vs+:printformat[pdf]","s:rootfile[test/tfs_abcd_'${SETS}']"},0)'
#	for SET in ${SETARR[@]}; do
#		root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_fit.txt","input/input_svj_mt_hist_tf_fit.txt","'${SETFILE}'"},{"OPTION","'$EXTRA'","vs:chosensets['${SET}']","vs:numers['${SET}']","vs:denoms['${SET}']","s+:printsuffix[_'${SET}'_res]","vs+:printformat[pdf]","s:ratiocalc[RelRes]","d:ratiomin[-0.5]","d:ratiomax[0.5]","s:rationame[residual (f-h)/f]"},1)'
#	done
	python applyTF.py -i test/tfs_abcd_${SETS}.root -h MTAK8_${SETARR[2]} -f ${POLY}_MTAK8_ratio__${SETARR[0]}__${SETARR[1]}
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_closure.txt","input/input_svj_mt_hist_full.txt","input/input_svj_ext_full_bkg_closure_abcd.txt"},{"OPTION","vs:chosensets['${SETARR[3]},${SETARR[3]}_${SETARR[2]}']","vs:numers['${SETARR[3]}']","vs:denoms['${SETARR[3]}_${SETARR[2]}']","s+:printsuffix[_'${SETS}'_closure]","vs+:printformat[pdf]"},1)'
}

TOYS=$1
SETFILE=input/input_svj_ext_full_bkg.txt
if [ -n "$TOYS" ]; then
	SETFILE=input/input_svj_ext_full_bkg_toys.txt
fi

SETLIST=(
highCutCR,vlowCutCR,vlowCutSR,highCutSR \
highCutCR,v2lowCutCR,v2lowCutSR,highCutSR \
lowCutCR,vlowCutCR,vlowCutSR,lowCutSR \
lowCutCR,v2lowCutCR,v2lowCutSR,lowCutSR \
)

PIDS=()

for SETS in ${SETLIST[@]}; do
	SETARR=()
	IFS="," read -a SETARR <<< "$SETS"
	HIST=input/input_svj_mt_hist_tf_fit.txt
	POLY="pol1"

	if [[ "$SETS" == *vlowCutSR,highCutSR ]]; then
		EXTRA="d:ratiomax[0.25]"
	elif [[ "$SETS" == *v2lowCutSR,highCutSR ]]; then
		EXTRA="d:ratiomax[1.0]"
	elif [[ "$SETS" == *vlowCutSR,lowCutSR ]]; then
		EXTRA="d:ratiomax[1.2]"
	elif [[ "$SETS" == *v2lowCutSR,lowCutSR ]]; then
		EXTRA="d:ratiomax[1.75]"
	fi

	runall >& log_makeABCD_${SETS}.log &
	PIDS[$SETS]=$!
done

for PID in ${PIDS[@]}; do
	wait $PID
done
