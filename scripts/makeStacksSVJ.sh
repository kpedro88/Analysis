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
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack.txt","input/input_svj_mt_full2.txt","'${SIG}'","input/input_svj_stack_ext_bkg.txt"},{"OPTION","string:extfilename[test/svj_dijethad_mt_full2.root]"},1)'
done
