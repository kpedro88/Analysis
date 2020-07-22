#!/bin/bash

TOPDIR=$CMSSW_BASE/src/Analysis
EOSDIR=""
# file prefix (not histo prefix)
PREFIX=MTAK8_dijetmtdetahadloosefull
NOMINAL=nominal
# histo prefix
HISTO=MTAK8_RA2bin
OUTNAME=$TOPDIR/test/datacard_bkg_data.root
SKIP=""

while getopts "d:p:n:h:o:k" opt; do
	case "$opt" in
		d) EOSDIR="$OPTARG"
		;;
		p) PREFIX="$OPTARG"
		;;
		n) NOMINAL="$OPTARG"
		;;
		h) HISTO="$OPTARG"
		;;
		o) OUTNAME="$OPTARG"
		;;
		k) SKIP=true
		;;
	esac
done

python $TOPDIR/processDatacardsSVJ.py -o $OUTNAME -i root://cmseos.fnal.gov/${EOSDIR} -f $(eos root://cmseos.fnal.gov ls ${EOSDIR} | grep ${PREFIX}_ | grep -v SVJ) -m $NOMINAL -p $HISTO -R 2 -t

xrdcp -f $OUTNAME root://cmseos.fnal.gov/${EOSDIR}

if [ -n "$SKIP" ]; then
	exit 0
fi

cd $CMSSW_BASE/src/Analysis/batch
./haddEOS.sh -d ${EOSDIR} -i datacard -g _ -k -r

