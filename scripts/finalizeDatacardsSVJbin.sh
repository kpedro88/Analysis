#!/bin/bash

EOSDIR=$1
PREFIX=$2
NOMINAL=$3
SUFF=$4
HISTO=MTAK8_RA2bin
ARGS="-t -i root://cmseos.fnal.gov/${EOSDIR} -s -m $NOMINAL -p $HISTO -f $(eos root://cmseos.fnal.gov ls ${EOSDIR} | grep ${PREFIX}_ | grep -v SVJ) $(eos root://cmseos.fnal.gov ls ${EOSDIR} | grep "${PREFIX}_SVJ_[2-4]000_20_0.3_peak")"
python processDatacardsSVJ.py $ARGS -o test/datacard${SUFF}_orig.root
python processDatacardsSVJ.py $ARGS -o test/datacard${SUFF}_rebin2.root -r="-c 2"
for sigma in 0.0 0.5 1.0 1.5 2.0; do
	OUTNAME=test/datacard${SUFF}_sigma${sigma}.root
	python processDatacardsSVJ.py $ARGS -o $OUTNAME -r="-s $sigma -w 2000 -r 2 -t 0.25"
done
