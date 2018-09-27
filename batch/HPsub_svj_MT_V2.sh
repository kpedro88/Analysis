#!/bin/bash

source exportProd.sh

JOBDIR=jobs
SEL=dijetmthad
QTY=event
CHECKARGS=""

#check arguments
while getopts "ks:q:" opt; do
	case "$opt" in
	k) CHECKARGS="${CHECKARGS} -k"
	;;
	s) SEL=$OPTARG
	;;
	q) QTY=$OPTARG
	;;
	esac
done

INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_${SEL}
STORE=$INDIR/hist
./SKcheck.sh ${CHECKARGS}

SETS=../input/input_svj_hp_sets.txt
INPUTS='"input/input_svj_hist.txt","input/input_svj_mc_cuts2.txt"'
for ((i=0; i < $(cat $SETS | wc -l); i+=2)); do
	LINE1=$(sed -n -e $((i+1))p ${SETS})
	LINE2=$(sed -n -e $((i+2))p ${SETS})
	SETNAME=$(echo "$LINE1" | cut -d$'\t' -f3)
	if [ "$SETNAME" != "QCD" ]; then continue; fi
	JOBNAME='svj_'${SEL}'_mc_cuts2_part'${i}
	OUTPUT='"OPTION","string:rootfile['${JOBNAME}']"'
	EXTRA='"SET","'${LINE1}'","'${LINE2}'"'
	./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} "$INPUTS" "$OUTPUT" "$EXTRA" "$JOBNAME"
done

