#!/bin/bash

source exportProd.sh

JOBDIR=jobs
VERSION=${RUN2PRODV}_v1
STORE1=root://cmseos.fnal.gov//store/user/lpcpmssm/Skims/${RUN2PRODV}
STORE2=root://cmseos.fnal.gov//store/user/lpcpmssm/Datacards/${VERSION}
SELS=signal,signal_JECup,signal_JECdown,signal_JERup,signal_JERdown,signal_genMHT,SLm,SLe,SLm_genMHT,SLe_genMHT
VARS=JECup,JECdown,JERup,JERdown,SLe,SLm,genMHT,SLe_genMHT,SLm_genMHT
REGION=default
OUTDIR=tree
INPUT=input/input_selection_pmssm.txt
EXTRAS='"OPTION","b:saveEventHists[0]"'
TYPE=Pmssm
PREFIX=Skim
INDIR1=root://cmseos.fnal.gov//store/user/lpcpmssm/${RUN2PRODV}
INDIR2=.
DCCONFIG=input/input_DC_config_RA2pmssm.txt
CHECKARGS=""
SUFFIX=""
YEARS=()
DRYRUN=""
MISSING=

#check arguments
while getopts "kx:y:dm" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		x) SUFFIX=$OPTARG
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		d) DRYRUN="echo"
		;;
		m) MISSING=1
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for YEAR in ${YEARS[@]}; do
	if [ "$YEAR" = 2016 ]; then
		SYSTS=nominal,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,prefireuncUp,prefireuncDown,puuncUp,puuncDown
	elif [ "$YEAR" = 2017 ]; then
		SYSTS=nominal,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,prefireuncUp,prefireuncDown,puuncUp,puuncDown
	elif [ "$YEAR" = 2018 ]; then
		SYSTS=nominal,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,puuncUp,puuncDown
	elif [ "$YEAR" = 2018HEM ]; then
		SYSTS=nominal,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,puuncUp,puuncDown
	else
		echo "Unknown year: $YEAR"
		exit 1
	fi

	# add fastsim-specific systs
	SYSTS=${SYSTS},btagCFuncUp,btagCFuncDown,ctagCFuncUp,ctagCFuncDown,mistagCFuncUp,mistagCFuncDown,jetiduncUp,jetiduncDown

	SNAME=${PREFIX}${TYPE}${YEAR}
	source export${SNAME}.sh
	# skip nonexistent ones
	if [[ $? -ne 0 ]]; then continue; fi

	JOBNAME1=skim_${TYPE}${YEAR}
	JOBNAME2=DC_ra2_${VERSION}_${REGION}_${TYPE}_${YEAR}

	LFNSTORE2=$(echo $STORE2 | sed 's~root://cmseos.fnal.gov/~~')
	NUMSAMP=
	for ((PROCESS=0; PROCESS < ${#SAMPLES[@]}; PROCESS++)); do
		SAMPLE=${SAMPLES[$PROCESS]}

		if [ -n "$MISSING" ]; then
			if eos root://cmseos.fnal.gov ls $LFNSTORE2/RA2bin_proc_${SAMPLE}.root >& /dev/null; then
				continue
			else
				NUMSAMP="$NUMSAMP,$PROCESS"
			fi
		fi

		echo 'KSkimDriver.C+("'$SAMPLE'","'$SELS'","'$INDIR1'",{"'$INPUT'"},{'"$EXTRAS"'},"'$OUTDIR'")' > jobs/input/macro_${JOBNAME1}_part${PROCESS}.txt
		echo 'MakeAllDCsyst.C+("'$SAMPLE'","'${INDIR2}'",{"'${DCCONFIG}'"},{},"'${REGION}'","'${SYSTS}'","'${VARS}'",{},1)' > jobs/input/macro_${JOBNAME2}_part${PROCESS}.txt
	done

	if [ -z "$MISSING" ]; then
		NUMSAMP=${#SAMPLES[@]}
	else
		if [ -z "$NUMSAMP" ]; then
			echo "No missing jobs in ${YEAR}"
			continue
		else
			NUMSAMP="Process in ${NUMSAMP:1}"
		fi
	fi
	$DRYRUN ./PMtemp.sh ${JOBDIR} ${STORE1} ${STORE2} ${JOBNAME1} ${JOBNAME2} ${OUTDIR} "${NUMSAMP}"
done
