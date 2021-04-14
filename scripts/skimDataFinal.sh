#!/bin/bash

YEARS=(
2016 \
2017 \
2018PRE \
2018POST \
)

declare -A SETS
SETS[2016]="JetHT_2016B JetHT_2016C JetHT_2016D JetHT_2016E JetHT_2016F JetHT_2016G JetHT_2016H"
SETS[2017]="JetHT_2017B JetHT_2017C JetHT_2017D JetHT_2017E JetHT_2017F"
SETS[2018PRE]="JetHT_2018A JetHT_2018B JetHT_2018C JetHT_2018D"
SETS[2018POST]="JetHT_2018A JetHT_2018B JetHT_2018C JetHT_2018D"

for YEAR in ${YEARS[@]}; do
	for SET in ${SETS[$YEAR]}; do
		root -b -l -q 'KSkimDriver.C+("'$SET'","lowCut,highCut,lowSVJ2,highSVJ2","root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijetmtdetahadloosefull",{"input/input_selection_svj_data'$YEAR'.txt","input/input_selection_svj_data.txt"},{},"test/paper/data/tree")'
	done
done
