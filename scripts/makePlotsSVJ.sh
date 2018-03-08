#!/bin/bash

SEL=dijethad

while getopts "s:" opt; do
	case "$opt" in
		s) SEL=$OPTARG
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
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_event.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_event2.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_thirdjet.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_shapeAK4.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_fwmAK4.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_shapeAK8.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_fwmAK8.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_jetpt.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)'
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_bothjet.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'","input/input_svj_flatten_bothjet.txt"},{},1)'
#	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_'${SEL}'.txt","input/input_svj_rocs.txt","input/input_svj_rocs_met.txt","input/input_svj_rocs_ext_bkg.txt","'${SIG}'"},{},1)'
done
