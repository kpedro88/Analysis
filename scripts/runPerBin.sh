#!/bin/bash

SAMPLES=(
T1tttt_1500_100_fast \
T1tttt_1200_800_fast \
T1bbbb_1500_100_fast \
T1bbbb_1000_900_fast \
T1qqqq_1400_100_fast \
T1qqqq_1000_800_fast \
T1tbtb_1500_100_fast \
T1tbtb_1100_700_fast \
T5qqqqVV_1400_100_fast \
T5qqqqVV_1000_800_fast \
T2tt_700_50_fast \
T2tt_300_200_fast \
T2bb_650_1_fast \
T2bb_500_300_fast \
T2qq_1000_100_fast \
T2qq_700_400_fast \
)

for SAMPLE in ${SAMPLES[@]}; do
	root -b -l -q 'plotPileupAccPerBin.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/scan/putest/tree_signal/tree_'${SAMPLE}'.root")'
done

