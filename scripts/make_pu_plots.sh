#!/bin/bash

SETS=(
T1tttt_1500_100_fast \
T1tttt_1200_800_fast \
T1bbbb_1500_100_fast \
T1bbbb_1000_900_fast \
T1qqqq_1400_100_fast \
T1qqqq_1000_800_fast \
T5qqqqVV_1400_100_fast \
T5qqqqVV_1000_800_fast \
T2tt_700_50_fast \
T2tt_300_200_fast \
T2bb_650_1_fast \
T2bb_500_300_fast \
T2qq_1000_100_fast \
T2qq_700_400_fast \
)

for SET in ${SETS[@]}; do
	root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/scan/supplementary",{"input/input_pu_cutflow_comp.txt","input/input_puset_'${SET}'.txt"},{"OPTION","string:rootfile[cutflows_PU_'${SET}']"},1)'
	root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/scan/supplementary",{"input/input_RA2bin_pu.txt","input/input_hist_RA2bin_pu.txt","input/input_puset_'${SET}'.txt"},{"OPTION","string:rootfile[RA2bin_PU_'${SET}']"},1)'
	root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/scan/supplementary",{"input/input_RA2bin_pu.txt","input/input_hist_1d_pu.txt","input/input_puset_'${SET}'.txt"},{"OPTION","string:rootfile[1D_PU_'${SET}']"},1)'
done

mv *.png plots/
mv *.root plots/
