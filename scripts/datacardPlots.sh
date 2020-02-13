#!/bin/bash

EXTDIR=$1
if [ -z "$EXTDIR" ]; then
	EXTDIR=Run2ProductionV17_v2
fi
EXTFILE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards/${EXTDIR}/datacard.root

HISTO=input/input_svj_mt_hist_full.txt

REGIONS=(
highCut \
lowCut \
highSVJ0 \
highSVJ1 \
highSVJ2 \
lowSVJ0 \
lowSVJ1 \
lowSVJ2 \
)
RNAMES=(
"high-^{}R_{T}" \
"low-^{}R_{T}" \
"high-SVJ0" \
"high-SVJ1" \
"high-SVJ2" \
"low-SVJ0" \
"low-SVJ1" \
"low-SVJ2" \
)

for ((i=0; i < ${#REGIONS[@]}; i++)); do
	REGION=${REGIONS[$i]}
	RNAME=${RNAMES[$i]}
	root -b -l -q 'KPlotDriver.C+("'$EXTDIR'",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_full_bdt_regions.txt","'$HISTO'","input/input_svj_ext_full_datacard_regions.txt"},{"OPTION","s:extfilename['${EXTFILE}']","s:exthisto_dir['${REGION}'_2018]","s+:printsuffix[_'${REGION}']","vs:extra_text['"${RNAME}"']","vs+:printformat[pdf]"},1)'
	echo "WINDOW: $EXTDIR $REGION"
	python getWindowYield.py -f $EXTFILE -d ${REGION}_2018 -s SVJ_mZprime3000_mDark20_rinv03_alphapeak -b Bkg
done

