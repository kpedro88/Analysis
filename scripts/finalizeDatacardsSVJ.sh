#!/bin/bash

EOSDIR=$1
# file prefix (not histo prefix)
PREFIX=$2
if [ -z "$PREFIX" ]; then
	PREFIX=MTAK8_dijetmtdetahadloosefull
fi
NOMINAL=$3
if [ -z "$NOMINAL" ]; then
	NOMINAL=nominal
fi
# histo prefix
HISTO=$4
if [ -z "$HISTO" ]; then
	HISTO=MTAK8_RA2bin
fi
OUTNAME=$5
if [ -z "$OUTNAME" ]; then
	OUTNAME=test/datacard_bkg_data.root
fi
SKIP=$6

python processDatacardsSVJ.py -o $OUTNAME -i root://cmseos.fnal.gov/${EOSDIR} -f $(eos root://cmseos.fnal.gov ls ${EOSDIR} | grep ${PREFIX}_ | grep -v SVJ) -m $NOMINAL -p $HISTO

if [ -n "$SKIP" ]; then
	exit 0
fi

xrdcp -f test/datacard_bkg_data.root root://cmseos.fnal.gov/${EOSDIR}
cd batch
./haddEOS.sh -d ${EOSDIR} -i datacard -g _ -k -r

