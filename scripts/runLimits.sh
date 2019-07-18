#!/bin/bash

declare -A REGIONS
REGIONS[cut]=S-Cut,B-Cut
REGIONS[bdt]=S-BDT1,S-BDT2,B-BDT1,B-BDT2
REGIONS[ubdt]=S-uBDT1,S-uBDT2,B-uBDT1,B-uBDT2

IFS="," read -a CONFIGS <<< "$1"
for CONFIG in ${CONFIGS[@]}; do
	echo $CONFIG
	FILE=MTAK8_dijetmtdetahadloosemf_fullbdtregions_${CONFIG}_split.root
	if [ ! -f "test/$FILE" ]; then
		ln -s /uscms_data/d3/pedrok/SUSY2015/analysis/v3/CMSSW_8_0_30/src/Analysis/test/${FILE} test/
	fi
#	for ANA in cut bdt ubdt; do
#	for ANA in bdt ubdt; do
	for ANA in bdt; do
		python datacard_svj.py -o svj_${ANA}${CONFIG} -f test/${FILE} -r ${REGIONS[$ANA]} -s SVJ
		./plotAll.sh svj_${ANA}${CONFIG}
	done
done
