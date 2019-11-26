#!/bin/bash

EOSDIR=$1

python processDatacardsSVJ.py -o test/datacard_bkg_data.root -i root://cmseos.fnal.gov/${EOSDIR} -f $(eos root://cmseos.fnal.gov ls ${EOSDIR} | grep MTAK8_dijetmtdetahadloosefull_ | grep -v SVJ)
xrdcp test/datacard_bkg_data.root root://cmseos.fnal.gov/${EOSDIR}
cd batch
./haddEOS.sh -d ${EOSDIR} -i datacard -g _ -k -r

