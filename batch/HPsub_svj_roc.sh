#!/bin/bash

source exportProd.sh

JOBDIR=jobs
SEL=dijetmtdetahadloosemf
QTY=event
CHECKARGS=""
TYPES=()
YEARS=()
OUTDIR=hist
FLATTEN=0
MOREINPUTS=()
EXTRA=""
DRYRUN=""

#check arguments
while getopts "ks:q:t:y:i:x:o:fd" opt; do
	case "$opt" in
	k) CHECKARGS="${CHECKARGS} -k"
	;;
	s) SEL=$OPTARG
	;;
	q) QTY=$OPTARG
	;;
	t) IFS="," read -a TYPES <<< "$OPTARG"
	;;
	y) IFS="," read -a YEARS <<< "$OPTARG"
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

INPUTS='"input/input_svj_hist.txt","input/input_svj_rocs_'${QTY}'.txt","input/input_svj_hp_sets.txt"'
if [ -n "$MOREINPUTS" ]; then
	for MOREINPUT in ${MOREINPUTS[@]}; do
		INPUTS="$INPUTS"',"'$MOREINPUT'"'
	done
fi
if [[ $FLATTEN -eq 1 ]]; then
	INPUTS="$INPUTS"',"input/input_svj_flatten_bothjet.txt"'
fi

for TYPE in ${TYPES[@]}; do
	for YEAR in ${YEARS[@]}; do
		SNAME=Hist${TYPE}${YEAR}
		source export${SNAME}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi

		JOBNAME=${OUTDIR}'_svj_'${TYPE}${YEAR}'_'${SEL}'_'${QTY}
		SLIST=${#SAMPLES[@]}
		for ((i=0; i < ${#SAMPLES[@]}; i++)); do
			SAMPLE=${SAMPLES[$i]}
			SJOBNAME=${JOBNAME}'_part'${i}
			OUTPUT='"OPTION","vstring:chosensets['${SAMPLE}']","string:rootfile['${SJOBNAME}']"'
			if [ -n "$EXTRA" ]; then
				OUTPUT="$OUTPUT","$EXTRA"
			fi
			echo 'KPlotDriver.C+("'"$INDIR"'",{'"$INPUTS"'},{'"$OUTPUT"'})' > jobs/input/macro_${SJOBNAME}.txt
		done

		$DRYRUN ./HPtemp.sh ${JOBDIR} ${INDIR} ${STORE} ${JOBNAME} "${SLIST}"
	done
done
