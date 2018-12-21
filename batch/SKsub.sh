#!/bin/bash

source exportProd.sh

JOBDIR=jobs
INPUT=input/input_selection.txt
SELTYPE=signal,LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,DYm_CleanVars,DYe_CleanVars
SELTYPE2=GJetLoose_CleanVars,GJetLDP_CleanVars,GJetLooseLDP_CleanVars,DYmLDP_CleanVars,DYeLDP_CleanVars
SELTYPE3=signal_JECup,signal_JECdown,signal_JERup,signal_JERdown
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
	for YEAR in ${YEARS[@]}; do
		source exportSkim${TYPE}${YEAR}.sh

		for SAMPLE in ${SAMPLES[@]}; do
			./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE}
			./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE2} ${INDIR} ${OUTDIR} ${STORE}
			if [ "$TYPE" = signal ]; then ./SKtemp.sh ${JOBDIR} ${INPUT} ${SAMPLE} ${SELTYPE2} ${INDIR} ${OUTDIR} ${STORE}; fi
		done
	done
done

