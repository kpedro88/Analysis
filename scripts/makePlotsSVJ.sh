#!/bin/bash

SEL=dijetmthad

PLOTS_ALL=(
0 \
1 \
2 \
3 \
4 \
5 \
6 \
7 \
8 \
)

LIST=""

while getopts "p:s:l" opt; do
	case "$opt" in
		p) if [ "$OPTARG" = all ]; then PLOTS=(${PLOTS_ALL[@]}); else IFS="," read -a PLOTS <<< "$OPTARG"; fi
		;;
		s) SEL=$OPTARG
		;;
		l) LIST="true"
		;;
	esac
done

SIGS=(
input/input_svj_rocs_ext_varyalpha.txt \
input/input_svj_rocs_ext_varymDark.txt \
input/input_svj_rocs_ext_varymZprime.txt \
input/input_svj_rocs_ext_varyrinv.txt \
)


for SIG in ${SIGS[@]}; do
CMDS=(
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_event.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_event2.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_thirdjet.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_shapeAK4.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_fwmAK4.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_shapeAK8.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_fwmAK8.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_jetpt.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_bothjet.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'","input/input_svj_flatten_bothjet.txt"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_met.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)' \
	'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_met_unskimmed.txt","input/input_svj_rocs_ext_qcd.txt","'${SIG}'"},{},1)' \
)

	if [ -n "$LIST" ]; then
		for ((i=0; i < ${#CMDS[@]}; i++)); do
			echo "$i ${CMDS[$i]}"
		done
		exit 0
	fi

	for PLOT in ${PLOTS[@]}; do
		root -b -l -q ${CMDS[$PLOT]}
	done
done
