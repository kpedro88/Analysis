#!/bin/bash

#run 3 at a time
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1000_compressed_trig1_plot9xx.txt",1)' > T1bbbb_1000_compressed_trig1_plot9xx_log.txt 2>&1 &
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1000_compressed_trig2_plot9xx.txt",1)' > T1bbbb_1000_compressed_trig2_plot9xx_log.txt 2>&1 &
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1000_compressed_trig3_plot9xx.txt",1)' > T1bbbb_1000_compressed_trig3_plot9xx_log.txt 2>&1
