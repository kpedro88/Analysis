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
SIGTOYNAME=""
SETDATA=
WINDOW=true
NEWBINS=
FITPLOT=""
FITFILE=""
FITSETDAT=""

while getopts "d:f:s:onymwbtT:F" opt; do
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
		   SIGTOYNAME=$OPTARG
		;;
		F) FITPLOT=true
		   FITFILE=input/input_svj_mt_fit_toy.txt
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
	SETDATA=input/input_svj_ext_full_datacard_data.txt
fi
if [ -n "$TOY" ]; then
	if [ -n "$FITPLOT" ]; then
		SETDATA=input/input_svj_ext_full_datacard_toy_fit.txt
	else
		SETDATA=input/input_svj_ext_full_datacard_toy.txt
	fi
fi
if [ -n "$SIGTOY" ]; then
	EXTRA=',"s+:printsuffix[_'$SIGTOYNAME']"'
	if [ -n "$FITPLOT" ]; then
		SETDATA=input/input_svj_ext_full_datacard_toy_fit.txt
		FITSETDAT=data_$SIGTOYNAME
		FITFILE=$(echo $FITFILE | sed 's/toy/'$SIGTOYNAME'/')
	else
		EXTRA="$EXTRA"',"b:yieldnorm[1]","s:yieldref[dnorm_'$SIGTOYNAME']","vs+:chosensets[data_'$SIGTOYNAME',data2_'$SIGTOYNAME',dnorm_'$SIGTOYNAME']"'
		SETDATA=input/input_svj_ext_full_datacard_sigtoy.txt
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
#highSVJ0 \
#highSVJ1 \
#lowSVJ0 \
#lowSVJ1 \
)
RNAMES=(
"high-^{}R_{T}" \
"low-^{}R_{T}" \
"high-SVJ2" \
"low-SVJ2" \
#"high-SVJ0" \
#"high-SVJ1" \
#"low-SVJ0" \
#"low-SVJ1" \
)

MAXPLOTS=${#REGIONS[@]}
if [ -n "$MINPLOTS" ]; then
	MAXPLOTS=4
fi

for ((i=0; i < $MAXPLOTS; i++)); do
	REGION=${REGIONS[$i]}
	RNAME=${RNAMES[$i]}
	if [ -n "$FITPLOT" ]; then
		root -b -l -q 'KPlotDriver.C+("'$EXTDIR'",{"input/input_svj_stack_dijetmtdetahad_2017.txt","'$FITFILE'","'$HISTO'","'$SETDATA'"},{"OPTION","s:extfilename['${EXTFILE}']","s:exthisto_dir['${REGION}'_2018]","s+:printsuffix[_'${REGION}${SUFF}']","vs:extra_text['"${RNAME}"']","vs+:printformat[pdf]","vs+:chosensets['$FITSETDAT']","vs+:numers['$FITSETDAT']","vs:denoms['$FITSETDAT']","vs:fits[fmain_'$REGION']"'"$EXTRA"'},1)'
	else
		root -b -l -q 'KPlotDriver.C+("'$EXTDIR'",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_full_bdt_regions.txt","'$HISTO'","'$SETDATA'","input/input_svj_ext_full_datacard_regions'$SETSUFF'.txt"},{"OPTION","s:extfilename['${EXTFILE}']","s:exthisto_dir['${REGION}'_2018]","s+:printsuffix[_'${REGION}${SUFF}']","vs:extra_text['"${RNAME}"']","vs+:printformat[pdf]","vs:numers[]","vs:denoms[]"'"$EXTRA"'},1)'
		if [ -n "$WINDOW" ]; then
			echo "WINDOW: $EXTDIR $REGION"
			python getWindowYield.py -f $EXTFILE -d ${REGION}_2018 -s SVJ_mZprime3100_mDark20_rinv03_alphapeak -b Bkg
		fi
	fi
done

