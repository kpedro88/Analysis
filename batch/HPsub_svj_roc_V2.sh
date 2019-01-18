#!/bin/bash

source exportProd.sh

JOBDIR=jobs
SEL=dijetmthad
QTY=event
CHECKARGS=""
SETS=sets

#check arguments
while getopts "ks:q:t:" opt; do
	case "$opt" in
	k) CHECKARGS="${CHECKARGS} -k"
	;;
	s) SEL=$OPTARG
	;;
	q) QTY=$OPTARG
	;;
	t) SETS=$OPTARG
	;;
	esac
done

INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/tree_${SEL}
STORE=$INDIR/hist
./SKcheck.sh ${CHECKARGS}

SETFILE=../input/input_svj_hp_${SETS}.txt

INPUTS='"input/input_svj_hist.txt","input/input_svj_rocs_'${QTY}'.txt"'
for ((i=0; i < $(cat $SETFILE | wc -l); i+=2)); do
	LINE1=$(sed -n -e $((i+1))p ${SETFILE})
	LINE2=$(sed -n -e $((i+2))p ${SETFILE})
	SETNAME=$(echo "$LINE1" | cut -d$'\t' -f3)
	JOBNAME='svj_'${SETS}'_'${SEL}'_'${QTY}'_part'${i}
	OUTPUT='"OPTION","string:rootfile['${JOBNAME}']"'
	EXTRA='"SET","'${LINE1}'","'${LINE2}'"'
	
	echo 'KPlotDriver.C+("'"$INDIR"'",{'"$INPUTS"'},{'"$EXTRA"','"$OUTPUT"'})' > jobs/input/macro_${JOBNAME}.txt

	./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} ${JOBNAME}
done

