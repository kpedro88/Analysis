#!/bin/bash

EXTDIR=Run2ProductionV17_v2
OVERLAY=""
SETSUFF=""
NORM=""
YIELD=""
HISTO=input/input_svj_mt_hist_full.txt
SUFF=""
MINPLOTS=""
TOY=""
SETDATA=
WINDOW=true
NEWBINS=

while getopts "d:f:s:onymwbt" opt; do
	case "$opt" in
		d) EXTDIR=$OPTARG
		;;
		f) EXTFILE=$OPTARG
		;;
		o) OVERLAY=true
		;;
		n) NORM=true
		;;
		y) YIELD=true
		;;
		s) SUFF=$OPTARG
		;;
		m) MINPLOTS=true
		;;
		w) WINDOW=""
		;;
		b) NEWBINS=true
		;;
		t) TOY=true
		;;
	esac
done

if [ -z "$EXTFILE" ]; then
	EXTFILE=root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards/${EXTDIR}/datacard.root
else
	EXTDIR=.
fi
if [ -n "$OVERLAY" ]; then
	SETSUFF=_overlay
fi
if [ -n "$NORM" ]; then
	EXTRA=',"b:unitnorm[1]","d:ymin_min[1e-6]","i:rebin[2]"'
fi
if [ -n "$YIELD" ]; then
	EXTRA=',"b:yieldnorm[1]","s:yieldref[data]"'
	SETDATA='"input/input_svj_ext_full_datacard_data.txt",'
fi
if [ -n "$TOY" ]; then
	SETDATA='"input/input_svj_ext_full_datacard_toy.txt",'
fi
if [ -n "$NEWBINS" ]; then
	EXTRA="$EXTRA"',"s:ytitle[events / GeV]","d:ymin_min[1e-6]"'
fi

REGIONS=(
highCut \
lowCut \
highSVJ2 \
lowSVJ2 \
highSVJ0 \
highSVJ1 \
lowSVJ0 \
lowSVJ1 \
)
RNAMES=(
"high-^{}R_{T}" \
"low-^{}R_{T}" \
"high-SVJ2" \
"low-SVJ2" \
"high-SVJ0" \
"high-SVJ1" \
"low-SVJ0" \
"low-SVJ1" \
)

MAXPLOTS=${#REGIONS[@]}
if [ -n "$MINPLOTS" ]; then
	MAXPLOTS=4
fi

for ((i=0; i < $MAXPLOTS; i++)); do
	REGION=${REGIONS[$i]}
	RNAME=${RNAMES[$i]}
	root -b -l -q 'KPlotDriver.C+("'$EXTDIR'",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_full_bdt_regions.txt","'$HISTO'",'"$SETDATA"'"input/input_svj_ext_full_datacard_regions'$SETSUFF'.txt"},{"OPTION","s:extfilename['${EXTFILE}']","s:exthisto_dir['${REGION}'_2018]","s+:printsuffix[_'${REGION}${SUFF}']","vs:extra_text['"${RNAME}"']","vs+:printformat[pdf]","vs:numers[]","vs:denoms[]"'"$EXTRA"'},1)'
	if [ -n "$WINDOW" ]; then
		echo "WINDOW: $EXTDIR $REGION"
		python getWindowYield.py -f $EXTFILE -d ${REGION}_2018 -s SVJ_mZprime3100_mDark20_rinv03_alphapeak -b Bkg
	fi
done

