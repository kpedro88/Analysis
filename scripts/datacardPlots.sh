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
SIGTOY=""
SETDATA=
WINDOW=true
NEWBINS=
FITPLOT=""
FITNAMETMP=""
FITSETSIG=""
FITSETDAT=""

while getopts "d:f:s:onymwbtTF" opt; do
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
		T) SIGTOY=true
		;;
		F) FITPLOT=true
		   HISTO=input/input_svj_mt_hist_full_fit.txt
		   FITNAMETMP=input/input_svj_mt_fit_toy_REGION.txt
		   FITSETSIG=SVJ_2100_20_0.3_peak,SVJ_3100_20_0.3_peak,SVJ_4100_20_0.3_peak
		   FITSETDAT=data
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
	if [ -n "$FITPLOT" ]; then
		SETDATA='"input/input_svj_ext_full_datacard_toy_fit.txt"'
	else
		SETDATA='"input/input_svj_ext_full_datacard_toy.txt",'
	fi
fi
if [ -n "$SIGTOY" ]; then
	if [ -n "$FITPLOT" ]; then
		SETDATA='"input/input_svj_ext_full_datacard_toy_fit.txt"'
		FITSETDAT=data_sig
		FITNAMETMP=$(echo $FITNAMETMP | sed 's/toy/sigtoy/')
		EXTRA=',"s+:printsuffix[_sigtoy]"'
	else
		EXTRA=',"b:yieldnorm[1]"'
		SETDATA='"input/input_svj_ext_full_datacard_sigtoy.txt",'
	fi
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
	if [ -n "$FITPLOT" ]; then
		FITFILE=$(echo $FITNAMETMP | sed 's/REGION/'$REGION'/')
		root -b -l -q 'KPlotDriver.C+("'$EXTDIR'",{"input/input_svj_stack_dijetmtdetahad_2017.txt","'$FITFILE'","'$HISTO'",'"$SETDATA"'},{"OPTION","s:extfilename['${EXTFILE}']","s:exthisto_dir['${REGION}'_2018]","s+:printsuffix[_'${REGION}${SUFF}']","vs:extra_text['"${RNAME}"']","vs+:printformat[pdf]","vs:chosensets['$FITSETDAT,$FITSETSIG']","vs:numers['$FITSETDAT,$FITSETSIG']","vs:denoms['$FITSETDAT']"'"$EXTRA"'},1)'
	else
		root -b -l -q 'KPlotDriver.C+("'$EXTDIR'",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_full_bdt_regions.txt","'$HISTO'",'"$SETDATA"'"input/input_svj_ext_full_datacard_regions'$SETSUFF'.txt"},{"OPTION","s:extfilename['${EXTFILE}']","s:exthisto_dir['${REGION}'_2018]","s+:printsuffix[_'${REGION}${SUFF}']","vs:extra_text['"${RNAME}"']","vs+:printformat[pdf]","vs:numers[]","vs:denoms[]"'"$EXTRA"'},1)'
		if [ -n "$WINDOW" ]; then
			echo "WINDOW: $EXTDIR $REGION"
			python getWindowYield.py -f $EXTFILE -d ${REGION}_2018 -s SVJ_mZprime3100_mDark20_rinv03_alphapeak -b Bkg
		fi
	fi
done

