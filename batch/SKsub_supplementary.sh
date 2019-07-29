#!/bin/bash

source exportProd.sh

JOBDIR=jobs
JOBTYPE=suppskim
INPUT=input/input_selection_supplementary.txt
SELTYPE=signal,signalMinusNJet,signalMinusHT,signalMinusMHT
INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan
OUTDIR=tree
STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/scan/supplementary
CHECKARGS=""
DRYRUN=""

#check arguments
while getopts "kd" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		d) DRYRUN="echo"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

SNAME=SkimSupp
source export${SNAME}.sh

$DRYRUN ./SKtemp.sh ${JOBDIR} ${INPUT} ${SNAME} ${#SAMPLES[@]} ${SELTYPE} ${INDIR} ${OUTDIR} ${STORE} ${JOBTYPE}
