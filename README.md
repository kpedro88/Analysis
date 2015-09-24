# Analysis

## Skimming

[input\_selection.txt](input/input\_selection.txt) defines all the available selections, variations, and samples, as well as common global options.

To run interactively, applying the "signal" selection to the "T1tttt\_1500\_100" sample and writing output trees to a folder "tree\_test\_${SELECTION}":
```
root -b -q -l 'KSkimDriver.C+("input/input_selection.txt","T1tttt_1500_100","signal","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV1","tree_test")'
```

To recompile the driver without running (preparing for batch submission):
```
root -b -q -l 'KSkimDriver.C++()'
```

To submit jobs to Condor:
```
./SKsub.sh
```

### Scanning

The SUSY FastSim signal samples contain multiple model points per file. Before skimming, these samples should be scanned to separate each model point into a separate file.

To run the scanner interactively:
```
root -b -q -l 'KScanDriver.C+("input/input_scan.txt","T1bbbb_mGluino-1000-1025_mLSP-1to975-1000","root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Ntuples/RA2sync_Sept23_2015")'
```

To recompile the driver without running (preparing for batch submission):
```
root -b -q -l 'KScanDriver.C++()'
```

## Plotting

To plot yields vs. RA2 bin (where the binning can be defined in the input file) in the signal region and save the plot as an image:
```
root -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV1/tree_signal","input/input_RA2bin.txt",1)'
```
Omitting the last argument will display the plot without saving it.

## Datacard creation

To save the individual histograms for each signal and background process to a ROOT file, for the creation of datacards for limit setting:
```
root -b -l -q 'MakeAllDC.C+("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV1")'
```
This macro calls the macro `KPlotDriverDC.C`, which is a modification of the macro from the previous section
that includes an extra input argument to specify the name of the output ROOT file.
These macros use modified input files that split up each background process (rather than stacking them together, as done for plotting).
The W+jets and ttbar processes are added together in the input file. The macro adds together the two single-lepton control region files
after they are generated.
