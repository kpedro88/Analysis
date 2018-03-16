#!/bin/bash

for SAMPLE in QCD SVJ_3000_20_0.3_0.2 SVJ_1000_20_0.3_0.2 SVJ_2000_20_0.3_0.2 SVJ_4000_20_0.3_0.2 SVJ_3000_1_0.3_0.2 SVJ_3000_50_0.3_0.2 SVJ_3000_100_0.3_0.2 SVJ_3000_20_0.1_0.2 SVJ_3000_20_0.5_0.2 SVJ_3000_20_0.7_0.2 SVJ_3000_20_0.3_0.1 SVJ_3000_20_0.3_0.5 SVJ_3000_20_0.3_1; do
	declare -A PIDS
	for VAR in metvars1 metvars2; do
		for MT in mt_norm mt_nonorm; do
			root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_hist.txt","input/input_svj_sets_'${VAR}'.txt","input/input_svj_'${MT}'.txt"},{"OPTION","s:extfilename[test/unskimmed/MTAK8_'${SAMPLE}'.root]","vs:extra_text['${SAMPLE}']","s+:printsuffix[_'${SAMPLE}']"},1)' 2>&1 &
			PIDS["$SAMPLE"]=$!
		done
	done
	for PID in ${PIDS[@]}; do
		wait $PID
	done
done
