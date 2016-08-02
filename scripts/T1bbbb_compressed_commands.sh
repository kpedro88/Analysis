#!/bin/bash

#run 3 at a time
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1000_compressed_trig1_plot9xx.txt",1)' > T1bbbb_1000_compressed_trig1_plot9xx_log.txt 2>&1 &
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1000_compressed_trig2_plot9xx.txt",1)' > T1bbbb_1000_compressed_trig2_plot9xx_log.txt 2>&1 &
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1000_compressed_trig3_plot9xx.txt",1)' > T1bbbb_1000_compressed_trig3_plot9xx_log.txt 2>&1
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1000_compressed_trig1_plot8xx.txt",1)' > T1bbbb_1000_compressed_trig1_plot8xx_log.txt 2>&1 &
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1000_compressed_trig2_plot8xx.txt",1)' > T1bbbb_1000_compressed_trig2_plot8xx_log.txt 2>&1 &
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1000_compressed_trig3_plot8xx.txt",1)' > T1bbbb_1000_compressed_trig3_plot8xx_log.txt 2>&1
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1400_compressed_trig1_plot.txt",1)' > T1bbbb_1400_compressed_trig1_plot_log.txt 2>&1 &
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1400_compressed_trig2_plot.txt",1)' > T1bbbb_1400_compressed_trig2_plot_log.txt 2>&1 &
root -b -q -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signalLooseHT250","input/input_RA2bin_T1bbbb_1400_compressed_trig3_plot.txt",1)' > T1bbbb_1400_compressed_trig3_plot_log.txt 2>&1
