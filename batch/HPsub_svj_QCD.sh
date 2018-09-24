#!/bin/bash

source exportProd.sh

JOBDIR=jobs
CHECKARGS=""

#check arguments
while getopts "k" opt; do
	case "$opt" in
	k) CHECKARGS="${CHECKARGS} -k"
	;;
	esac
done

INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV14
STORE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/qcd/hist2
./SKcheck.sh ${CHECKARGS}

SETS=../input/input_sets_hp_qcd_sep.txt
INPUTS='"input/input_svj_hist.txt","input/input_svj_qcd_met_weight_sel.txt","input/input_svj_qcd_met_weight_hist.txt"'
for ((i=0; i < $(cat $SETS | wc -l); i+=2)); do
	LINE1=$(sed -n -e $((i+1))p ${SETS})
	LINE2=$(sed -n -e $((i+2))p ${SETS})
	SETNAME=$(echo "$LINE1" | cut -d$'\t' -f3)
	JOBNAME='svj_qcd_met_weight_part'${i}
	OUTPUT='"OPTION","string:rootfile['${JOBNAME}']"'
	EXTRA='"SET","'${LINE1}'","'${LINE2}'"'
	./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} "$INPUTS" "$OUTPUT" "$EXTRA" "$JOBNAME"
done

