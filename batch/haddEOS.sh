#!/bin/bash

INPUT=""
DIR=""
SUFF=""
SEARCH=""
RUN=0
UPDATE=0
VERBOSE=0

#check arguments
while getopts "d:i:x:g:ruv" opt; do
	case "$opt" in
	r) RUN=1
	;;
	u) UPDATE=1
	;;
	x) SUFF=$OPTARG
	;;
	d) DIR=$OPTARG
	;;
	i) INPUT=$OPTARG
	;;
	g) SEARCH=$OPTARG
	;;
	v) VERBOSE=1
	;;
	esac
done

XRDIR=root://cmseos.fnal.gov/$DIR
#convert input into array
IFS=',' read -r -a SAMPLES <<< "$INPUT"; unset IFS
for BASE in ${SAMPLES[@]}; do
	echo $BASE
	
	#check to see if anything needs to be hadded
	IFS=$'\n' LGFILES=($(xrdfs root://cmseos.fnal.gov ls ${DIR} | grep "${BASE}${SEARCH}")); unset IFS
	if [[ ${#LGFILES[@]} -eq 0 ]]; then
		echo "nothing to hadd for $BASE"
		continue
	fi
	
	ALLFILES=""
	for FILE in ${LGFILES[@]}; do
		ALLFILES="${ALLFILES} ${XRDIR}/`basename ${FILE}`"
	done
	
	#include base file when updating
	if [[ $UPDATE -eq 1 ]]; then
		ALLFILES="${XRDIR}/${BASE}.root ${ALLFILES}"
	fi

	#setup filename	
	TMPFILE=${BASE}.root
	if [[ -n "$SUFF" ]]; then
		TMPFILE=${BASE}_${SUFF}.root
	fi

	#dryrun (list nfiles) is default
	if [[ $RUN -eq 0 ]]; then
		if [[ $VERBOSE -eq 0 ]]; then
			echo ${ALLFILES} | wc -w
		else
			sed 's/ /\n\t/g' <<< "Input:$ALLFILES"
			echo -e "Output:\n\t$TMPFILE"
		fi
		continue
	fi
	
	#hadd to tmp file
	hadd ${TMPFILE} ${ALLFILES}
	
	#check exit code
	HADDEXIT=$?
	if [[ $HADDEXIT -ne 0 ]]; then
		rm ${TMPFILE}
		echo "exit code $HADDEXIT, skipping xrdcp"
		exit $HADDEXIT
	fi
	
	#copy hadded file to eos
	xrdcp -f ${TMPFILE} ${XRDIR}/

	#check exit code
	XRDEXIT=$?
	if [[ $XRDEXIT -ne 0 ]]; then
		rm ${TMPFILE}
		echo "exit code $XRDEXIT, failure in xrdcp"
		exit $XRDEXIT
	fi	
	
	#remove original files (only if hadd and xrdcp succeeded)
	for FILE in ${LGFILES[@]}; do
		xrdfs root://cmseos.fnal.gov/ rm ${FILE}
	done
	
	#remove tmp file
	rm ${TMPFILE}
done
