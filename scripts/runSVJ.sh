#!/bin/bash

function discrs(){
	CONFIG=$1
	for MVA in BDT uBDT; do
		root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijetmtdetahadmf",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_sel_bdtnew'${CONFIG}'.txt","input/input_svj_sel_bdt_incl.txt","input/input_svj_sel_full_bdt_regions.txt","input/input_svj_discr_full_bdtnew_regions.txt","input/input_svj_sets_full_bdt_regions.txt"},{"OPTION","vs:selections[S-'${MVA}']","s+:printsuffix[_'${CONFIG}'_'${MVA}'_stack]"},1)' >& log_discr_${MVA}_${CONFIG}.log
	done
}

# this makes the MT distribution from scratch
function mt(){
	CONFIG=$1
	for MVA in BDT uBDT; do
		root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijetmtdetahadmf",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_sel_bdtnew'${CONFIG}'.txt","input/input_svj_sel_bdt2.txt","input/input_svj_sel_full_bdt_regions.txt","input/input_svj_mt_full_bdt_regions.txt","input/input_svj_sets_full_bdt_regions.txt"},{"OPTION","s+:printsuffix[_'$CONFIG'_S-'$MVA'2]","vs:selections[S-'$MVA']","vs:extra_text[S-'$MVA'2 ('$CONFIG')]"},1)' >& log_mt_S-${MVA}2_${CONFIG}.log
	done
}

# this makes the MT distribution from the split full region histograms
function regions(){
	CONFIG=$1
	root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijetmtdetahadmf",{"input/input_svj_stack_generic.txt","input/input_svj_sel_bdtnew'${CONFIG}'.txt","input/input_svj_sel_bdt_incl.txt","input/input_svj_sel_full_bdt_regions.txt","input/input_svj_mt_fullnew_hist.txt"},{"OPTION","s:rootfile[test/MTAK8_dijetmtdetahadmf_fullbdtregions_'${CONFIG}']"},0)' >& log_fullbdtregions${CONFIG}.log
	python splitRegionsSVJ.py test/MTAK8_dijetmtdetahadmf_fullbdtregions_${CONFIG}.root
	for MVA in BDT uBDT; do
		root -b -l -q 'KPlotDriver.C+(".",{"input/input_svj_stack_dijetmtdetahad_2017.txt","input/input_svj_mt_full_bdt_regions.txt","input/input_svj_ext_full_'${MVA}'_regions.txt"},{"OPTION","s:extfilename[test/MTAK8_dijetmtdetahadmf_fullbdtregions_'${CONFIG}'_split.root]","s+:printsuffix[_'$CONFIG'_S-'$MVA'2]","vs:extra_text[S-'$MVA'2 ('$CONFIG')]"},1)' >& log_mt_S-${MVA}2_${CONFIG}.log
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

#check arguments
while getopts "C:m:" opt; do
	case "$opt" in
		C) IFS="," read -a CONFIGS <<< "$OPTARG"
		;;
		m) if [ "$OPTARG" = all ]; then MODES=(${MODES_ALL[@]}); else IFS="," read -a MODES <<< "$OPTARG"; fi
		;;
	esac
done


for CONFIG in ${CONFIGS[@]}; do
	# get BDT
	for x in TMVA_bdt_weights.xml TMVA_ubdt_weights.xml; do
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
