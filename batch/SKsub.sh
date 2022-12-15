#!/bin/bash

source exportProd.sh

JOBDIR=jobs
JOBTYPE=skim
SELTYPE1=signal
SELTYPE2=signalSideband,LDP,SLm,SLe,SLmLDP,SLeLDP,GJet_CleanVars,GJetLDP_CleanVars,GJetLoose_CleanVars,GJetLooseLDP_CleanVars,DYm_CleanVars,DYe_CleanVars,DYmLDP_CleanVars,DYeLDP_CleanVars
SELTYPE3=signal_JECup,signal_JECdown,signal_JERup,signal_JERdown
SELTYPE4=signal_genMHT,SLm,SLe,SLm_genMHT,SLe_genMHT
OUTDIR=tree
CHECKARGS=""
TYPES=()
YEARS=()
DRYRUN=""
COMMON=0

#check arguments
while getopts "ky:t:dc" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
		d) DRYRUN="echo"
		;;
		c) COMMON=1
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	if [ "$TYPE" = Pmssm ]; then
		INPUT=input/input_selection_pmssm.txt
		INDIR=root://cmseos.fnal.gov//store/user/lpcpmssm/${RUN2PRODV}
		STORE=root://cmseos.fnal.gov//store/user/lpcpmssm/Skims/${RUN2PRODV}
	else
		INPUT=input/input_selection.txt
		INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
		STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}
	fi

	# temporary vars
	SELS=$SELTYPE1
	TMPINDIR="$INDIR"
	TMPSTORE="$STORE"

	# special settings
	if [ "$TYPE" != Fast ] && [ "$TYPE" != Pmssm ]; then
		SELS=$SELS,$SELTYPE2
	fi
	if [ "$TYPE" = Signal ] || [ "$TYPE" = Fast ] || [ "$TYPE" = Pmssm ]; then
		SELS=$SELS,$SELTYPE3
	fi
	if [ "$TYPE" = Fast ] || [ "$TYPE" = Pmssm ]; then
		SELS=$SELS,$SELTYPE4
	fi
	if [ "$TYPE" = Fast ]; then
		TMPINDIR="$INDIR"/scan
		TMPSTORE="$STORE"/scan
	fi
	if [ "$TYPE" = Data ]; then
		SELS=$SELS,signalUnblind
	fi

	# common overrides sels & adds extras
	DO_PMSSM=0
	if [ "$TYPE" = Pmssm ]; then
		if [ "$COMMON" -eq 1 ]; then
			SELS=common,common_JECup,common_JECdown,common_JERup,common_JERdown,common_genMHT
			EXTRAS='"OPTION","b:saveTree[0]","b:saveCutflow[0]"'
		else
			DO_PMSSM=1
			EXTRAS='"OPTION","b:saveEventHists[0]"'
		fi
	fi

	for YEAR in ${YEARS[@]}; do
		SNAME=Skim${TYPE}${YEAR}
		source export${SNAME}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi

		JOBNAME=${JOBTYPE}_${SNAME}
		if [ "$TYPE" = Pmssm ] && [ "$COMMON" -eq 1 ]; then
			JOBNAME=${JOBNAME}_common
		fi
		for ((PROCESS=0; PROCESS < ${#SAMPLES[@]}; PROCESS++)); do
			SAMPLE=${SAMPLES[$PROCESS]}
			echo 'KSkimDriver.C+("'$SAMPLE'","'$SELS'","'$TMPINDIR'",{"'$INPUT'"},{'"$EXTRAS"'},"'$OUTDIR'")' > jobs/input/macro_${JOBNAME}_part${PROCESS}.txt
		done

		$DRYRUN ./SKtemp.sh ${JOBDIR} ${STORE} ${JOBNAME} ${OUTDIR} ${#SAMPLES[@]} $DO_PMSSM
	done
done

