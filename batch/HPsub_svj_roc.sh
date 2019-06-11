#!/bin/bash

source exportProd.sh

JOBDIR=jobs
SEL=dijetmtdetahadloosemf
QTY=event
CHECKARGS=""
SETS=()
OUTDIR=hist
FLATTEN=0
MOREINPUTS=()
EXTRA=""
DRYRUN=""

#check arguments
while getopts "ks:q:t:i:x:o:fd" opt; do
	case "$opt" in
	k) CHECKARGS="${CHECKARGS} -k"
	;;
	s) SEL=$OPTARG
	;;
	q) QTY=$OPTARG
	;;
	t) IFS="," read -a SETS <<< "$OPTARG"
	;;
	i) IFS="," read -a MOREINPUTS <<< "$OPTARG"
	;;
	x) EXTRA="$OPTARG"
	;;
	o) OUTDIR=$OPTARG
	;;
	f) FLATTEN=1
	;;
	d) DRYRUN="echo"
	;;
	esac
done

INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/tree_${SEL}
STORE=$INDIR/$OUTDIR
./SKcheck.sh ${CHECKARGS}

INPUTS='"input/input_svj_hist.txt","input/input_svj_rocs_'${QTY}'.txt"'
if [ -n "$MOREINPUTS" ]; then
	for MOREINPUT in ${MOREINPUTS[@]}; do
		INPUTS="$INPUTS"',"'$MOREINPUT'"'
	done
fi
if [[ $FLATTEN -eq 1 ]]; then
	INPUTS="$INPUTS"',"input/input_svj_flatten_bothjet.txt"'
fi

for SET in ${SETS[@]}; do
	SETFILE=../input/input_svj_hp_${SETS}.txt
	for ((i=0; i < $(cat $SETFILE | wc -l); i+=2)); do
		LINE1=$(sed -n -e $((i+1))p ${SETFILE})
		LINE2=$(sed -n -e $((i+2))p ${SETFILE})
		SETNAME=$(echo "$LINE1" | cut -d$'\t' -f3)
		JOBNAME=${OUTDIR}'_svj_'${SET}'_'${SEL}'_'${QTY}'_part'${i}
		OUTPUT='"OPTION","string:rootfile['${JOBNAME}']"'
		if [ -n "$EXTRA" ]; then
			OUTPUT="$OUTPUT","$EXTRA"
		fi
		SETOPT='"SET","'${LINE1}'","'${LINE2}'"'
	
		echo 'KPlotDriver.C+("'"$INDIR"'",{'"$INPUTS"'},{'"$SETOPT"','"$OUTPUT"'})' > jobs/input/macro_${JOBNAME}.txt

		$DRYRUN ./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} ${JOBNAME}
	done
done
