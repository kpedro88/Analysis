#!/bin/bash

EXTFILE=$1
if [ -z "$EXTFILE" ]; then
	EXTFILE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards/Run2ProductionV17_v2/datacard.root
fi

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
	root -b -l -q 'KPlotDriver.C+("Run2ProductionV17_v2",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_full_bdt_regions.txt","input/input_svj_ext_full_datacard_regions.txt"},{"OPTION","s:extfilename['${EXTFILE}']","s:exthisto_dir['${REGION}'_2018]","s+:printsuffix[_'${REGION}']","vs:extra_text['"${RNAME}"']","vs+:printformat[pdf]"},1)'
done

