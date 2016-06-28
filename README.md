# Analysis

## Recompiling

To recompile all drivers, testing code changes or preparing for batch submission:
```
./recompile.sh
```
Add the flag `-f` to force recompilation of all drivers.

## Skimming

[input\_selection.txt](input/input\_selection.txt) defines all the available selections, variations, and samples, as well as common global options.

To run interactively, applying the "signal" selection to the "T1tttt\_1500\_100" sample and writing output trees to a folder "test/tree\_${SELECTION}":
```
root -b -q -l 'KSkimDriver.C+("input/input_selection.txt","T1tttt_1500_100","signal","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV8","test/tree")'
```

To remake the input list of sets automatically, for data and MC:
```
python makeSkimInput.py -r input/dict_skim.py -w input/input_sets_skim.txt -e batch/exportSkim.sh -n 50
python makeSkimInput.py -r input/dict_skim_signal.py -w input/input_sets_skim_signal.txt -e batch/exportSkimSignal.sh -n 50
python makeSkimInput.py -r input/dict_skim_data.py -w input/input_sets_skim_data.txt -e batch/exportSkimData.sh --data -n 50 -f /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV8
```
Note: this script uses the python file lists in [TreeMaker/Production/python](https://github.com/TreeMaker/TreeMaker/tree/Run2/Production/python) to determine the number of files to chain together for each sample. Make sure to follow the [TreeMaker](https://github.com/TreeMaker/TreeMaker) installation instructions so this information is accessible.

To submit jobs to Condor (add the flag `-k` to reuse the existing CMSSW tarball):
```
cd batch
./SKsub.sh
./SKsub_signal.sh
./SKsub_data.sh
./SKsub_fast.sh
```
Note: `SKsub_fast.sh` should only be run after the scanning step, below, is completed.

After the skims finish, some may need to be hadded (split or extended samples):
```
./hadd_skims.sh -r
```

<a name="cutflow"></a>Skimmed ROOT files include a histogram called "cutflow" which contains the raw number of events passing each selector in the selection, in order. To print a cutflow table from a skimmed ROOT file:
```
root -b -l -q 'KCutflowDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV8/tree_signal/tree_T1tttt_1500_100.root")'
```
Additional arguments can be added to enable printing statistical errors and to change the number of significant figures printed.

### Scanning

The SUSY FastSim signal samples contain multiple model points per file. Before skimming, these samples should be scanned to separate each model point into a separate file.

To run the scanner interactively:
```
root -b -q -l 'KScanDriver.C+("input/input_scan.txt","T1bbbb_mGluino-1000-1025_mLSP-1to975-1000","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV8")'
```

To make the input lists (.txt and .sh) of FastSim samples automatically:
```
python makeScanInput.py -n 30
```
The last argument splits the scan input list into multiple blocks (each containing `n` ntuple files) for batch submission.  
Note: as above, this script uses the python file lists in [TreeMaker/Production/python](https://github.com/TreeMaker/TreeMaker/tree/Run2/Production/python).

To submit jobs to Condor:
```
cd batch
./SCsub.sh
```

After the jobs finish, the split output files should be combined (in batch mode due to the typically large number of hadd operations necessary):
```
./HSsub.sh -r
```

To make the input lists of model points automatically for skimming, plotting, and datacards, after the scan jobs are finished and combined:
```
python makeFastInput.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV8/scan/ -n 10
```
The last argument splits the datacard input list into multiple lists (each containing `n` model points) for batch submission.

<a name="combined"></a>A separate script is available to create a "combined" model by adding together multiple signal models with different weights.
This script creates input lists for plotting and datacards (the existing skims are re-used).
```
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV8/scan/ -n 5 -s T1tbtb,T1tbbb,T1bbbb -w 0.25,0.5,0.25 -x T1tbtbT1tbbbT1bbbb
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV8/scan/ -n 5 -s T1tbtb,T1tbtt,T1tttt -w 0.25,0.5,0.25 -x T1tbtbT1tbttT1tttt
```
If weights are not explicitly specified (using the `-w` option), each sample is weighted equally. The weights are always normalized so sum(weights) = 1.

## Plotting

To plot yields vs. RA2 bin (where the binning can be defined in the input file) in the signal region and save the plot as an image:
```
root -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV8/tree_signal","input/input_RA2bin.txt",1)'
```
Omitting the last argument will display the plot without saving it.

## Datacard creation

To save the individual histograms for each signal and background process to a ROOT file, for the creation of datacards for limit setting:
```
root -b -l -q 'MakeAllDC.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV8")'
```
This macro calls the macro `KPlotDriverDC.C`, which is a modification of the macro from the previous section
that includes an extra input argument to specify the name of the output ROOT file.
These macros use modified input files that split up each background process (rather than stacking them together, as done for plotting).
The W+jets and ttbar processes are added together in the input file. The macro adds together the two single-lepton control region files
after they are generated.

To save the histogram for data to a ROOT file for datacard creation:
```
root -b -l -q 'MakeAllDCdata.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV8","signal")'
```

### Signal systematics

To save the individual histograms for the FullSim signal processes to a ROOT file for each different variation of systematic uncertainty and control region contamination:
```
root -b -l -q 'MakeAllDCsyst.C+(0,"root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV8")'
```
This macro calls the macro `KPlotDriverDCsyst.C`, which is a modification of the macro from the previous section
that includes extra input arguments to specify the list of input samples and the name and direction of the systematic variation (up or down).

To check specific systematics or contaminations, there are extra arguments. E.g., to check only the PDF uncertainty and no contaminations:
```
root -b -l -q 'MakeAllDCsyst.C+(0,"root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV8","pdfunc","")'
```

Because of the large number of model points in the FastSim signal samples, batch mode should be used to run over them:
```
cd batch
./DCsub.sh
```

After the jobs finish, the split output files should be combined:
```
./hadd_DC.sh -r
```

Currently available uncertainties:
* Pileup reweighting
* PDF weights
* Scale weights (renormalization/factorization)
* initial state radiation
* trigger efficiency statistical
* trigger efficiency systematic
* Jet energy corrections
* b-tagging scale factors
* c-tagging scale factors
* mistagging scale factors
* b-tagging correction factors (fastsim only)
* c-tagging correction factors (fastsim only)
* mistagging correction factors (fastsim only)

Signal contamination is checked in the LowDeltaPhi, Gamma+Jets, and SingleLepton control regions.
