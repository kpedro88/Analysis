#!/bin/bash

source exportProd.sh

JOBDIR=jobs
REGION=default
CHECKARGS=""
YEARS=()
TYPES=()
DRYRUN=""
VERSION=${RUN2PRODV}_v2
CONFIG=""
NOSYST=""

#check arguments
while getopts "ky:t:c:v:r:sd" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
		c) CONFIG=$OPTARG
		;;
		v) VERSION=$OPTARG
		;;
		r) REGION=$OPTARG
		;;
		s) NOSYST=true
		;;
		d) DRYRUN="echo"
		;;
	esac
done

STORE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards/${VERSION}
./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	PREFIX=DC
	if [ "$TYPE" = SVJScan ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/scan
		DCCONFIG=input/input_DC_config_SVJscan.txt
		if [ "$YEAR" != Run2 ]; then
			PREFIX=Skim
		fi
	elif [ "$TYPE" = SVJ ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
		DCCONFIG=input/input_DC_config_SVJsig.txt
	elif [ "$TYPE" = SVJData ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
		DCCONFIG=input/input_DC_config_SVJdata.txt
	elif [ "$TYPE" = SVJBkg ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
		DCCONFIG=input/input_DC_config_SVJbkg.txt
	else
		echo "Unknown or unsupported type: $TYPE"
		exit 1
	fi

	if [ -n "$CONFIG" ]; then
		DCCONFIG=$CONFIG
	fi

	if [[ -z "$NOSYST" && ("$TYPE" = SVJ  ||  "$TYPE" = SVJScan) ]]; then
		SYSTS=nominal,trigfnuncUp,trigfnuncDown,puuncUp,puuncDown,pdfalluncUp,pdfalluncDown
		VARS=JECup,JECdown,JERup,JERdown
	else
		SYSTS=nominal
		VARS=none
	fi

	for YEAR in ${YEARS[@]}; do
		SNAME=${PREFIX}${TYPE}${YEAR}
		source export${SNAME}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi

		JOBNAME=DC_svj_${VERSION}_${REGION}_${TYPE}_${YEAR}
		echo 'MakeAllDCsyst.C+("NEWSAMPLE","'${INDIR}'",{"'${DCCONFIG}'"},{},"'${REGION}'","'${SYSTS}'","'${VARS}'")' > jobs/input/macro_${JOBNAME}.txt

		$DRYRUN ./DCtemp.sh ${JOBDIR} ${STORE} ${JOBNAME} ${SNAME} ${#SAMPLES[@]} ${TYPE} ${YEAR}
	done
done
