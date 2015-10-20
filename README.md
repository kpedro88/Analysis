# Analysis

## Skimming

[input\_selection.txt](input/input\_selection.txt) defines all the available selections, variations, and samples, as well as common global options.

To run interactively, applying the "signal" selection to the "T1tttt\_1500\_100" sample and writing output trees to a folder "tree\_test/tree\_${SELECTION}":
```
root -b -q -l 'KSkimDriver.C+("input/input_selection.txt","T1tttt_1500_100","signal","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV3","tree_test/tree")'
```

To remake the input list of sets automatically, for data and MC:
```
python makeSkimInput.py -r input/dict_skim.txt -w input/input_sets_skim.txt
python makeSkimInput.py -r input/dict_skim_data.txt -w input/input_sets_skim_data.txt
```
Note: this script uses the python file lists in [TreeMaker/Production/python](https://github.com/TreeMaker/TreeMaker/tree/Run2/Production/python) to determine the number of files to chain together for each sample. Make sure to follow the [TreeMaker](https://github.com/TreeMaker/TreeMaker) installation instructions so this information is accessible.

To recompile the driver without running (preparing for batch submission):
```
root -b -q -l 'KSkimDriver.C++()'
```

To submit jobs to Condor:
```
./SKsub.sh
./SKsub_data.sh
./SKsub_fast.sh
```
Note: `SKsub_fast.sh` should only be run after the scanning step, below, is completed.

### Scanning

The SUSY FastSim signal samples contain multiple model points per file. Before skimming, these samples should be scanned to separate each model point into a separate file.

To run the scanner interactively:
```
root -b -q -l 'KScanDriver.C+("input/input_scan.txt","T1bbbb_mGluino-1000-1025_mLSP-1to975-1000","root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Ntuples/RA2sync_Sept23_2015")'
```

To make the input lists (.txt and .sh) of FastSim samples automatically:
```
python makeScanInput.py
```
Note: as above, this script uses the python file lists in [TreeMaker/Production/python](https://github.com/TreeMaker/TreeMaker/tree/Run2/Production/python).

To recompile the driver without running (preparing for batch submission):
```
root -b -q -l 'KScanDriver.C++()'
```

To submit jobs to Condor:
```
./SCsub.sh
```

To make the input lists of model points automatically for skimming, plotting, and datacards, after the scan jobs finish:
```
python makeFastInput.py -d /eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV3/scan/
```

## Plotting

To plot yields vs. RA2 bin (where the binning can be defined in the input file) in the signal region and save the plot as an image:
```
root -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV3/tree_signal","input/input_RA2bin.txt",1)'
```
Omitting the last argument will display the plot without saving it.

## Datacard creation

To save the individual histograms for each signal and background process to a ROOT file, for the creation of datacards for limit setting:
```
root -b -l -q 'MakeAllDC.C+("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV3")'
```
This macro calls the macro `KPlotDriverDC.C`, which is a modification of the macro from the previous section
that includes an extra input argument to specify the name of the output ROOT file.
These macros use modified input files that split up each background process (rather than stacking them together, as done for plotting).
The W+jets and ttbar processes are added together in the input file. The macro adds together the two single-lepton control region files
after they are generated.

### Signal systematics

To save the individual histograms for the signal processes to a ROOT file for each different variation of systematic uncertainty:
```
root -b -l -q 'MakeAllDCsyst.C+(0,"root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV3")'
```
This macro calls the macro `KPlotDriverDCsyst.C`, which is a modification of the macro from the previous section
that includes extra input arguments to specify the name and direction of the systematic variation (up or down).
To run over fastsim signal samples, change the first argument to `1`.

Currently available uncertainties:
* b-tagging scale factors
* mistagging scale factors
* b-tagging correction factors (fastsim only)
* c-tagging correction factors (fastsim only)
* mistagging correction factors (fastsim only)
