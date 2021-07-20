#!/bin/bash

source exportProd.sh

JOBDIR=jobs
REGION=default
CHECKARGS=""
YEARS=()
TYPES=()
DRYRUN=""
VERSION=""
CONFIG=""
NOSYST=""
EXTRAS=""
NOMINAL=nominal

#check arguments
while getopts "ky:t:c:v:r:e:n:sd" opt; do
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
		e) EXTRAS="$OPTARG"
		;;
		s) NOSYST=true
		;;
		n) NOMINAL=$OPTARG
		;;
		d) DRYRUN="echo"
		;;
	esac
done

if [ -z "$VERSION" ]; then
	echo "Must specify version folder with -v"
	exit 1
fi

STORE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards/${VERSION}
SAFEVERSION=$(echo $VERSION | sed 's~/~_~')
./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	PREFIX=DC
	if [[ $TYPE == "SVJScan"* ]]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/scan
		DCCONFIG=input/input_DC_config_SVJscan.txt
	elif [ "$TYPE" = SVJ ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
		DCCONFIG=input/input_DC_config_SVJsig.txt
	elif [ "$TYPE" = SVJData ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
		DCCONFIG=input/input_DC_config_SVJdata.txt
	elif [[ "$TYPE" == "SVJBkg"* ]]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
		DCCONFIG=input/input_DC_config_SVJbkg.txt
	elif [ "$TYPE" = SVJBkgGJ ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
		DCCONFIG=input/input_DC_config_SVJbkg.txt
	elif [ "$TYPE" = SVJBkgQCDHT ]; then
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
		DCCONFIG=input/input_DC_config_SVJbkg.txt
	else
		echo "Unknown or unsupported type: $TYPE"
		exit 1
	fi

	if [ -n "$CONFIG" ]; then
		DCCONFIG=$CONFIG
	fi

	if [[ -z "$NOSYST" && ("$TYPE" == SVJ  ||  "$TYPE" == "SVJScan"*) ]]; then
		SYSTS=${NOMINAL},trigfnuncUp,trigfnuncDown,puuncUp,puuncDown,pdfalluncUp,pdfalluncDown,psisruncUp,psisruncDown,psfsruncUp,psfsruncDown
		VARS=JECup,JECdown,JERup,JERdown,JESup,JESdown
	else
		SYSTS=${NOMINAL}
		VARS=none
	fi

	for YEAR in ${YEARS[@]}; do
		SNAME=${PREFIX}${TYPE}${YEAR}
		source export${SNAME}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi

		JOBNAME=DC_svj_${SAFEVERSION}_${REGION}_${TYPE}_${YEAR}
		echo 'MakeAllDCsyst.C+("NEWSAMPLE","'${INDIR}'",{"'${DCCONFIG}'"},{'"$EXTRAS"'},"'${REGION}'","'${SYSTS}'","'${VARS}'")' > jobs/input/macro_${JOBNAME}.txt

		$DRYRUN ./DCtemp.sh ${JOBDIR} ${STORE} ${JOBNAME} ${SNAME} ${#SAMPLES[@]} ${TYPE} ${YEAR} ${NOMINAL}
	done
done
