#!/bin/bash

for YEAR in 2016 2017 2018 2018Pre 2018Post; do
	# generate input files and print list of globbed mb values
	PILEUP=($(python makePuInputs.py $YEAR))
	for PU in ${PILEUP[@]}; do
		root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17/tree_GJet_CleanVars",{"input/input_putest_'${YEAR}'_'${PU}'.txt","input/input_nvtx_GJets_'${YEAR}'.txt"},{},1)'
	done
done
