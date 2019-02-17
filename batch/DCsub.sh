#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}_v3
#SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,btagCFuncUp,btagCFuncDown,ctagCFuncUp,ctagCFuncDown,mistagCFuncUp,mistagCFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown,puaccuncUp,puaccuncDown
VARS=JECup,JECdown,JERup,JERdown,genMHT
CHECKARGS=""
SUFFIX=""
SKIPFILE=""
YEARS=()

#check arguments
while getopts "kx:y:s:" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		x) SUFFIX=$OPTARG
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		s) SKIPFILE=$OPTARG
		;;
  esac
done

./SKcheck.sh ${CHECKARGS}

if [ -n "$SKIPFILE" ]; then
	source ${SKIPFILE}
fi

for YEAR in ${YEARS[@]}; do
	if [ "$YEAR" = 2016 ]; then
		SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown,prefireuncUp,prefireuncDown
	elif [ "$YEAR" = 2017 ]; then
		SYSTS=nominal,scaleuncUp,scaleuncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown,prefireuncUp,prefireuncDown
	elif [ "$YEAR" = 2018 ]; then
		SYSTS=nominal,scaleuncUp,scaleuncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown
	elif [ "$YEAR" = 2018HEM ]; then
		SYSTS=nominal,scaleuncUp,scaleuncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,jetiduncUp,jetiduncDown,hemvetouncUp,hemvetouncDown
	else
		echo "Unknown year: $YEAR"
		exit 1
	fi

	if [ -n "$SUFFIX" ]; then
		source exportSkimFast${SUFFIX}${YEAR}.sh
	else
		source exportSkimFast${YEAR}.sh
	fi

	for SAMPLE in ${SAMPLES[@]}; do
		# check skips
		SKIPTHIS=""
		for SKIP in ${SKIPLIST[@]}; do
			if [[ $SAMPLE == "$SKIP"* ]]; then
				SKIPTHIS=yes
				break
			fi
		done
		if [[ -n "$SKIPTHIS" ]]; then
			continue
		fi
  
		./DCtemp.sh ${JOBDIR} ${INDIR} ${SYSTS} ${VARS} ${STORE} ${SAMPLE}
	done
done
