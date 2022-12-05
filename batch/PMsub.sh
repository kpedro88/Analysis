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

#check arguments
while getopts "kx:y:d" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		x) SUFFIX=$OPTARG
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		d) DRYRUN="echo"
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

	for ((PROCESS=0; PROCESS < ${#SAMPLES[@]}; PROCESS++)); do
		SAMPLE=${SAMPLES[$PROCESS]}
		echo 'KSkimDriver.C+("'$SAMPLE'","'$SELS'","'$INDIR1'",{"'$INPUT'"},{'"$EXTRAS"'},"'$OUTDIR'")' > jobs/input/macro_${JOBNAME1}_part${PROCESS}.txt
		echo 'MakeAllDCsyst.C+("'$SAMPLE'","'${INDIR2}'",{"'${DCCONFIG}'"},{},"'${REGION}'","'${SYSTS}'","'${VARS}'")' > jobs/input/macro_${JOBNAME2}_part${PROCESS}.txt
	done

	$DRYRUN ./PMtemp.sh ${JOBDIR} ${STORE1} ${STORE2} ${JOBNAME1} ${JOBNAME2} ${OUTDIR} ${#SAMPLES[@]}
done
