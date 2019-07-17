#!/bin/bash

function discrs(){
	CONFIG=$1
	for MVA in ${BDTNAMES[@]}; do
		root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijetmtdetahadloosemf",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_sel_'${SELBASE}${CONFIG}'.txt","input/input_svj_sel_bdt_incl.txt","input/input_svj_sel_full_bdt_regions.txt","input/input_svj_discr_full_bdtnew_regions.txt","input/input_svj_sets_full_bdt_regions.txt"},{"OPTION","vs:selections[pre-'${MVA}']","s+:printsuffix[_'${CONFIG}'_'${MVA}'_stack]","s:rootfile[test/bothjetAK8bdt__dijetmtdetahad_'${CONFIG}'_'${MVA}']","vs+:printformat[pdf]"},1)' >& log_discr_${MVA}_${CONFIG}.log
		# make ROC curves
		root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_rocs_hist_dijetmtdetahadloosemf.txt","input/input_svj_roc_bdt_ext.txt"},{"OPTION","s:extfilename[test/bothjetAK8bdt__dijetmtdetahad_'${CONFIG}'_'${MVA}'.root]","vs+:printformat[pdf]","s:rootfile[test/bothjetAK8bdt__dijetmtdetahad_'${CONFIG}'_'${MVA}'_roc]","s+:rocsuffix[_'${CONFIG}'_'${MVA}']"},1)';
	done
}

# this makes the MT distribution from scratch
function mt(){
	CONFIG=$1
	for MVA in ${BDTNAMES[@]}; do
		root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijetmtdetahadloosemf",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_sel_'${SELBASE}${CONFIG}'.txt","input/input_svj_sel_bdt2.txt","input/input_svj_sel_full_bdt_regions.txt","input/input_svj_mt_full_bdt_regions.txt","input/input_svj_sets_full_bdt_regions.txt"},{"OPTION","s+:printsuffix[_'$CONFIG'_S-'$MVA'2]","vs:selections[S-'$MVA']","vs:extra_text[S-'$MVA'2 ('$CONFIG')]"},1)' >& log_mt_S-${MVA}2_${CONFIG}.log
	done
}

# this makes the MT distribution from the split full region histograms
function regions(){
	CONFIG=$1
	root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijetmtdetahadloosemf",{"input/input_svj_stack_generic.txt","input/input_svj_sel_'${SELBASE}${CONFIG}'.txt","input/input_svj_sel_bdt_incl.txt","input/input_svj_sel_full_bdt_regions.txt","input/input_svj_mt_fullnew_hist.txt"},{"OPTION","s:rootfile[test/MTAK8_dijetmtdetahadloosemf_fullbdtregions_'${CONFIG}']","vs:selections['${BDTSELS}']"},0)' >& log_fullbdtregions${CONFIG}.log
	python splitRegionsSVJ.py test/MTAK8_dijetmtdetahadloosemf_fullbdtregions_${CONFIG}.root
	for MVA in ${BDTNAMES[@]}; do
		root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_full_bdt_regions.txt","input/input_svj_ext_full_'${MVA}'_regions.txt"},{"OPTION","s:extfilename[test/MTAK8_dijetmtdetahadloosemf_fullbdtregions_'${CONFIG}'_split.root]","s:exthisto_extra[S-'$MVA'2]","s+:printsuffix[_'$CONFIG'_S-'$MVA'2]","vs:extra_text[S-'$MVA'2 ('$CONFIG')]","vs+:printformat[pdf]"},1)' >& log_mt_S-${MVA}2_${CONFIG}.log
	done
}

CONFIGS=()
MODES_ALL=(
	discrs \
	regions \
	mt \
)
MODES=()
INDIR=/uscmst1b_scratch/lpc1/3DayLifetime/pedrok/bdt
BDTFILES=(bdt ubdt)
BDTNAMES=(BDT uBDT)
BDTSELS="B-BDT,S-BDT,B-uBDT,S-uBDT"
SELBASE="bdtnew"

#check arguments
while getopts "C:m:bo" opt; do
	case "$opt" in
		C) IFS="," read -a CONFIGS <<< "$OPTARG"
		;;
		m) if [ "$OPTARG" = all ]; then MODES=(${MODES_ALL[@]}); else IFS="," read -a MODES <<< "$OPTARG"; fi
		;;
		b)
			BDTFILES=(bdt)
			BDTNAMES=(BDT)
			BDTSELS="B-BDT,S-BDT"
		;;
		o)
			SELBASE="bdt"
		;;
	esac
done


for CONFIG in ${CONFIGS[@]}; do
	# get BDT
	for b in ${BDTFILES[@]}; do
		x=TMVA_${b}_weights.xml
		OUTFILE=test/${CONFIG}_${x}
		if ! [ -f "$OUTFILE" ]; then
			cp ${INDIR}/trainings_${CONFIG}/${x} $OUTFILE
		fi
	done

	declare -A PIDS
	for MODE in ${MODES[@]}; do
		echo $MODE $CONFIG
		$MODE $CONFIG &
		PIDS[$MODE]=$!
	done
	for PID in ${PIDS[@]}; do
		wait $PID
	done
done
