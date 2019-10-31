#!/bin/bash

source exportProd.sh

JOBDIR=jobs
JOBTYPE=skim
INPUT=input/input_selection_svj.txt
SELTYPE=dijetmtdetahadloose,dijetmtdetahadloosefull,dijetmthighdetahadloose,dijetmthighdetahadloosefull
# old skims: dijetmtdetahadloosemf,dijetlowmtdetahadloose,dijetlowmtdetahadloosemf,dijetmtdetamu,dijetmtdetaele
# old syst skims: dijetmtdetahadloosemf_JECup,dijetmtdetahadloosemf_JECdown,dijetmtdetahadloosemf_JERup,dijetmtdetahadloosemf_JERdown
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims
CHECKARGS=""
TYPES=()
YEARS=()
DRYRUN=""
SYST=""

#check arguments
while getopts "ky:t:sd" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
		s) SYST=true
		;;
		d) DRYRUN="echo"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	TMPINDIR="$INDIR"
	TMPSTORE="$STORE"
	if [ "$TYPE" = SVJScan ]; then
		TMPINDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/scan
		TMPSTORE="$STORE"/scan
	fi

	for YEAR in ${YEARS[@]}; do
		SNAME=Skim${TYPE}${YEAR}
		source export${SNAME}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi

		TMPSELTYPE=$SELTYPE
		SLIST=${#SAMPLES[@]}
		if [[ $TYPE == "Data" ]]; then
			SLIST=""
			for ((i=0; i < ${#SAMPLES[@]}; i++)); do
				if [[ ${SAMPLES[$i]} == *"JetHT"* ]]; then
					SLIST="$SLIST,$i"
				fi
			done
			# remove first char, prepend condor stuff
			SLIST="Process in ${SLIST:1}"
		elif ( [[ $TYPE == "SVJ" ]] || [[ $TYPE == "SVJScan" ]] ) && [ -n "$SYST" ]; then
			TMPSELTYPE=$TMPSELTYPE,dijetmtdetahadloosefull_JECup,dijetmtdetahadloosefull_JECdown,dijetmtdetahadloosefull_JERup,dijetmtdetahadloosefull_JERdown
		fi

		$DRYRUN ./SKtemp.sh ${JOBDIR} ${INPUT} ${SNAME} "${SLIST}" ${TMPSELTYPE} ${TMPINDIR} ${OUTDIR} ${TMPSTORE} ${JOBTYPE}
	done
done

