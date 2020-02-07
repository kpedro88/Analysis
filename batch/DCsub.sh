#!/bin/bash

source exportProd.sh

JOBDIR=jobs
VERSION=${RUN2PRODV}_v1
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Datacards/${VERSION}
VARS=JECup,JECdown,JERup,JERdown,SLe,SLm
REGION=default
CHECKARGS=""
SUFFIX=""
YEARS=()
TYPES=()
DRYRUN=""

#check arguments
while getopts "kx:y:t:d" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		x) SUFFIX=$OPTARG
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
		d) DRYRUN="echo"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	PREFIX=DC
	if [ "$TYPE" = Fast ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan
		PREFIX=Skim
		DCCONFIG=input/input_DC_config_RA2fast.txt
		VARS=${VARS}",genMHT,SLe_genMHT,SLm_genMHT"
	elif [ "$TYPE" = Signal ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}
		DCCONFIG=input/input_DC_config_RA2full.txt
	elif [ "$TYPE" = Data ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}
		DCCONFIG=input/input_DC_config_RA2data.txt
	else
		echo "Unknown or unsupported type: $TYPE"
		exit 1
	fi

	for YEAR in ${YEARS[@]}; do
		if [ "$TYPE" = Fast ] || [ "$TYPE" = Signal ]; then
			if [ "$YEAR" = 2016 ]; then
				SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,prefireuncUp,prefireuncDown,puuncUp,puuncDown
			elif [ "$YEAR" = 2017 ]; then
				SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,prefireuncUp,prefireuncDown,puuncUp,puuncDown
			elif [ "$YEAR" = 2018 ]; then
				SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,puuncUp,puuncDown
			elif [ "$YEAR" = 2018HEM ]; then
				SYSTS=nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,trigsystuncUp,trigsystuncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,puuncUp,puuncDown
			else
				echo "Unknown year: $YEAR"
				exit 1
			fi

			# add fastsim-specific systs
			if [ "$TYPE" = Fast ]; then
				SYSTS=${SYSTS},btagCFuncUp,btagCFuncDown,ctagCFuncUp,ctagCFuncDown,mistagCFuncUp,mistagCFuncDown,jetiduncUp,jetiduncDown
			fi
		else
			SYSTS=nominal
			VARS=none
		fi

		if [ -n "$SUFFIX" ]; then
			SNAME=${PREFIX}${TYPE}${SUFFIX}${YEAR}
		else
			SNAME=${PREFIX}${TYPE}${YEAR}
		fi

		source export${SNAME}.sh

		JOBNAME=DC_ra2_${VERSION}_${REGION}_${TYPE}_${YEAR}
		echo 'MakeAllDCsyst.C+("NEWSAMPLE","'${INDIR}'",{"'${DCCONFIG}'"},{},"'${REGION}'","'${SYSTS}'","'${VARS}'")' > jobs/input/macro_${JOBNAME}.txt

		$DRYRUN ./DCtemp.sh ${JOBDIR} ${STORE} ${JOBNAME} ${SNAME} ${#SAMPLES[@]} ${TYPE} ${YEAR}          
	done
done
