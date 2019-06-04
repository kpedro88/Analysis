#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}_v6
VARS=JECup,JECdown,JERup,JERdown,genMHT,SLe,SLm,SLe_genMHT,SLm_genMHT
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
		SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,btagCFuncUp,btagCFuncDown,ctagCFuncUp,ctagCFuncDown,mistagCFuncUp,mistagCFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown,prefireuncUp,prefireuncDown,puuncUp,puuncDown
	elif [ "$YEAR" = 2017 ]; then
		SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,btagCFuncUp,btagCFuncDown,ctagCFuncUp,ctagCFuncDown,mistagCFuncUp,mistagCFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown,prefireuncUp,prefireuncDown,puuncUp,puuncDown
	elif [ "$YEAR" = 2018 ]; then
		SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,btagCFuncUp,btagCFuncDown,ctagCFuncUp,ctagCFuncDown,mistagCFuncUp,mistagCFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown,puuncUp,puuncDown
	elif [ "$YEAR" = 2018HEM ]; then
		SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,btagCFuncUp,btagCFuncDown,ctagCFuncUp,ctagCFuncDown,mistagCFuncUp,mistagCFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown,puuncUp,puuncDown
	else
		echo "Unknown year: $YEAR"
		exit 1
	fi

	if [ -n "$SUFFIX" ]; then
		SNAME=SkimFast${SUFFIX}${YEAR}
	else
		SNAME=SkimFast${YEAR}
	fi

	source export${SNAME}.sh

	$DRYRUN ./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS} ${VARS} ${STORE} ${SNAME} ${#SAMPLES[@]}
done
