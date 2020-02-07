#!/bin/bash

EOSDIR=$1
# file prefix (not histo prefix)
PREFIX=$2
if [ -z "$PREFIX" ]; then
	PREFIX=MTAK8_dijetmtdetahadloosefull
fi

python processDatacardsSVJ.py -o test/datacard_bkg_data.root -i root://cmseos.fnal.gov/${EOSDIR} -f $(eos root://cmseos.fnal.gov ls ${EOSDIR} | grep ${PREFIX}_ | grep -v SVJ)
xrdcp -f test/datacard_bkg_data.root root://cmseos.fnal.gov/${EOSDIR}
cd batch
./haddEOS.sh -d ${EOSDIR} -i datacard -g _ -k -r

