#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE1=signal,signalSideband,LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,GJetLDP_CleanVars
SELTYPE2=GJetLoose_CleanVars,GJetLooseLDP_CleanVars,DYm_CleanVars,DYe_CleanVars,DYmLDP_CleanVars,DYeLDP_CleanVars
SELTYPE3=signal_JECup,signal_JECdown,signal_JERup,signal_JERdown
SELTYPE4=signal_genMHT,LDP_genMHT,SLm_genMHT,SLe_genMHT,SLmLDP_genMHT,SLeLDP_genMHT
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}
CHECKARGS=""
TYPES=()
YEARS=()

#check arguments
while getopts "ky:t:" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	# temporary vars
	SELS=(
"$SELTYPE1" \
	)
	TMPINDIR="$INDIR"
	TMPSTORE="$STORE"

	# special settings
	if [ "$TYPE" != Fast ]; then
		SELS+=("$SELTYPE2")
	fi
	if [ "$TYPE" = Signal ] || [ "$TYPE" = Fast ]; then
		SELS+=("$SELTYPE3")
	fi
	if [ "$TYPE" = Fast ]; then
		SELS+=("$SELTYPE4")
		TMPINDIR="$INDIR"/scan
		TMPSTORE="$STORE"/scan
	fi

	for YEAR in ${YEARS[@]}; do
		source exportSkim${TYPE}${YEAR}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi

		for SAMPLE in ${SAMPLES[@]}; do
			for SEL in ${SELS[@]}; do
				echo ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SEL} ${TMPINDIR} ${OUTDIR} ${TMPSTORE}
			done
		done
	done
done

