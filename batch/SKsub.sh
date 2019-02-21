#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE1=signal
SELTYPE2=signalSideband,LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,GJetLDP_CleanVars,GJetLoose_CleanVars,GJetLooseLDP_CleanVars,DYm_CleanVars,DYe_CleanVars,DYmLDP_CleanVars,DYeLDP_CleanVars
SELTYPE3=signal_JECup,signal_JECdown,signal_JERup,signal_JERdown
SELTYPE4=signal_genMHT
SELTYPE5=SLm,SLe,SLm_genMHT,SLe_genMHT
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}
CHECKARGS=""
TYPES=()
YEARS=()
DRYRUN=""

#check arguments
while getopts "ky:t:d" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
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
	# temporary vars
	SELS=$SELTYPE1
	TMPINDIR="$INDIR"
	TMPSTORE="$STORE"

	# special settings
	if [ "$TYPE" != Fast ]; then
		SELS=$SELS,$SELTYPE2
	fi
	if [ "$TYPE" = Signal ] || [ "$TYPE" = Fast ]; then
		SELS=$SELS,$SELTYPE3
	fi
	if [ "$TYPE" = Fast ]; then
		SELS=$SELS,$SELTYPE4
		if [[ $SAMPLE == "T1t"* ]] || [[ $SAMPLE == "T5qqqqVV"* ]] || [[ $SAMPLE == "T2tt"* ]]; then
			SELS=$SELS,$SELTYPE5
		fi
		TMPINDIR="$INDIR"/scan
		TMPSTORE="$STORE"/scan
	fi
	if [ "$TYPE" = Data ]; then
		SELS=$SELS,signalUnblind
	fi

	for YEAR in ${YEARS[@]}; do
		source exportSkim${TYPE}${YEAR}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi

		for SAMPLE in ${SAMPLES[@]}; do
			$DRYRUN ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELS} ${TMPINDIR} ${OUTDIR} ${TMPSTORE}
		done
	done
done

