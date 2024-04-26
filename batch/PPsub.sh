#!/bin/bash

source exportProd.sh

JOBDIR=jobs
VERSION=${RUN2PRODV}_v1
STORE=root://cmseos.fnal.gov//store/user/lpcpmssm/Datacards/${VERSION}
PREFIX="RA2bin_signal_"
REGION=default
DCCONFIG=input/input_DC_config_RA2pmssm.txt
CHECKARGS=""
DRYRUN=""
MATCH=

#check arguments
while getopts "kdM:" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		d) DRYRUN="echo"
		;;
		M) MATCH="$OPTARG"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

LFN=$(echo $STORE | sed 's~root://cmseos.fnal.gov/~~')
for SAMPLE in $(eos root://cmseos.fnal.gov ls $LFN | grep $PREFIX); do
	SAMPLE=$(echo $SAMPLE | sed 's/'$PREFIX'//; s/.root//')
	JOBNAME=proc_${SAMPLE}

	if [ -n "$MATCH" ]; then
		if ! [[ "$SAMPLE" == *"$MATCH"* ]]; then
			continue
		fi
	fi

	echo 'MakeAllDCproc.C+("'$SAMPLE'","'${STORE}'",{"'${DCCONFIG}'"},{},"'${REGION}'",1)' > jobs/input/macro_${JOBNAME}.txt

	$DRYRUN ./PPtemp.sh ${JOBDIR} ${STORE} ${JOBNAME}
done
