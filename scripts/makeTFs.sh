#!/bin/bash

runall() {
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_fit.txt","input/input_svj_mt_hist_tf_fit.txt","'${SETFILE}'"},{"OPTION","'$EXTRA'","vs:chosensets['${SETS}']","vs:numers['${SETARR[0]}']","vs:denoms['${SETARR[1]}']","s+:printsuffix[_'${SETS}']","i:npanel[2]","b:balance_panels[1]","vs+:printformat[pdf]","s:rootfile[test/tfs_'${SOURCENAME}${SETS}']","i:rebin[2]"},1)'
	if [ -n "$STOPEARLY" ]; then
		return 0
	fi
	RNAME=MTAK8_ratio__${SETARR[0]}__${SETARR[1]}
	FITS="soft_${RNAME}"
	SETNUMER=${SETARR[1]}soft
	if [ -n "$BDT" ]; then
		FITS="pol1_${RNAME}"
		SETNUMER=${SETARR[1]}pol1
	fi
	python applyTF.py -i test/tfs_${SOURCENAME}${SETS}.root -h MTAK8_${SETARR[1]} -f $FITS -t MTAK8_${SETARR[0]}
	root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_tf_closure.txt","input/input_svj_mt_hist_full.txt","input/input_svj_ext_full_bkg_closure.txt"},{"OPTION","vs:chosensets['${SETARR[0]},${SETNUMER}']","vs:numers['${SETNUMER}']","vs:denoms['${SETARR[0]}']","s+:printsuffix[_'${SETS}'_closure]","vs+:printformat[pdf]","s:extfilename['test/applytfs_${SOURCENAME}${SETS}.root']"},1)'
}

SOURCE=$1
SOURCENAME=$SOURCE
if [ -n "$SOURCENAME" ]; then
	SOURCENAME=${SOURCENAME}_
fi
STOPEARLY=""
SETFILE=input/input_svj_ext_full_bkg.txt
if [[ "$SOURCE" == "toys" ]]; then
	SETFILE=input/input_svj_ext_full_bkg_toys.txt
elif [[ "$SOURCE" == "notrig" ]]; then
	SETFILE=input/input_svj_ext_full_bkg_notrig.txt
	STOPEARLY=true
elif [[ "$SOURCE" == "trig" ]]; then
	SETFILE=input/input_svj_ext_full_bkg_trig.txt
fi

SETLIST=(
highCutSR,highCutCR \
lowCutSR,lowCutCR \
vlowCutSR,vlowCutCR \
v2lowCutSR,v2lowCutCR \
highSVJ2,highSVJ0 \
lowSVJ2,lowSVJ0 \
vlowSVJ2,vlowSVJ0 \
v2lowSVJ2,v2lowSVJ0 \
)

PIDS=()

for SETS in ${SETLIST[@]}; do
	SETARR=()
	IFS="," read -a SETARR <<< "$SETS"
	HIST=input/input_svj_mt_hist_tf_fit.txt
	EXTRA="d:ratiomax[7]"
	BDT=""
	if [[ "$SETS" == *"SVJ0" ]]; then
		EXTRA="d:ratiomax[0.05]"
		BDT=true
	fi

	runall >& log_makeTFs_${SOURCENAME}${SETS}.log &
	PIDS[$SETS]=$!
done

for PID in ${PIDS[@]}; do
	wait $PID
done
