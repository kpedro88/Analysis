# Analysis

Important note: the current version of this code uses ROOT6 and C++11 features. It is not compatible with older versions of ROOT, i.e. ROOT5.

## Setup

To recompile all drivers, for testing code changes or preparing batch submission:
```
./recompile.sh
```
Add the flag `-f` to force recompilation of all drivers.

To link scripts from the [scripts](./scripts) directory into the main directory:
```
./setup.sh
```

To regenerate the `NtupleClass.h` header:
```
./makeNtupleClass.sh
```
The file [sourceNtupleClass.sh](./scripts/sourceNtupleClass.sh) can be modified to change which ROOT file(s) to use as a template.

<!--
## Skimming

[input\_selection.txt](input/input\_selection.txt) defines all the available selections, variations, and samples, as well as common global options.

To run interactively, applying the "signal" selection to the "T1tttt\_1500\_100" sample and writing output trees to a folder "test/tree\_${SELECTION}":
```
root -b -q -l 'KSkimDriver.C+("T1tttt_1500_100","signal","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV12",{"input/input_selection.txt"},{},"test/tree")'
```

To submit jobs to Condor (add the flag `-k` to reuse the existing CMSSW tarball):
```
cd batch
./SKsub.sh
./SKsub_signal.sh
./SKsub_data.sh
```

After the skims finish, some may need to be hadded (split or extended samples):
```
./hadd_skims.sh -r
```

<a name="cutflow"></a>Skimmed ROOT files include a histogram called "cutflow" which contains the raw number of events passing each selector in the selection, in order. To print a cutflow table from a skimmed ROOT file:
```
root -b -l -q 'KCutflowDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/tree_signal/tree_T1tttt_1500_100.root")'
```
Additional arguments can be added to enable printing statistical errors and to change the number of significant figures printed.
-->

## Plotting

To compare different mass distributions for a given signal and save the plot as an image:
```
root -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/GenProductionV1",{"input/input_masses.txt","input/input_masses_sets_mZprime-3000_mDark-20_rinv-0.3_alpha-0.2_n-10000.txt"},{},1)'
```
Omitting the last argument will display the plot without saving it.
