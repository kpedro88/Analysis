# Analysis

Important note: the current version of this code uses ROOT6 and C++11 features. It is not compatible with older versions of ROOT, i.e. ROOT5.

Currently, the skimming functionality (copying vector branches) does not work in ROOT version 6.10 (CMSSW_9_4_X).
Instead, ROOT version 6.06 (CMSSW_8_0_X) should be used.

## Setup

```
cmsrel CMSSW_8_0_30
cd CMSSW_8_0_30/src
cmsenv
git clone git@github.com:kpedro88/Analysis
cd Analysis
./setupScripts.sh
./setupTM.sh
./setupXML.sh
```

The script `setupScripts.sh` links useful tools kept in the `scripts` folder for easier use.

The script `setupTM.sh` compiles specific parts of [TreeMaker](https://github.com/TreeMaker/TreeMaker) that are used in this code.

The script `setupXML.sh` downloads the [pugixml](https://github.com/zeux/pugixml) library (for use with the custom BDT evaluator).

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
root -b -q -l 'KSkimDriver.C+("T1tttt_2000_100_MC2017","signal","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17",{"input/input_selection.txt"},{},"test/tree")'
```

To remake the input list of sets automatically, for data and MC:
```
python makeSkimInput.py -r input/dict_skim_mc_2016.py -w input/input_sets_skim_mc_2016.txt -e batch/exportSkimMC2016.sh -n 50
python makeSkimInput.py -r input/dict_skim_mc_2017.py -w input/input_sets_skim_mc_2017.txt -e batch/exportSkimMC2017.sh -n 50
python makeSkimInput.py -r input/dict_skim_mc_2018.py -w input/input_sets_skim_mc_2018.txt -e batch/exportSkimMC2018.sh -n 50
python makeSkimInput.py -r input/dict_skim_signal_2016.py -w input/input_sets_skim_signal_2016.txt -e batch/exportSkimSignal2016.sh -n 50
python makeSkimInput.py -r input/dict_skim_signal_2017.py -w input/input_sets_skim_signal_2017.txt -e batch/exportSkimSignal2017.sh -n 50
python makeSkimInput.py -r input/dict_skim_data_2016.py -w input/input_sets_skim_data_2016.txt -e batch/exportSkimData2016.sh --data -n 50 -f /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17
python makeSkimInput.py -r input/dict_skim_data_2017.py -w input/input_sets_skim_data_2017.txt -e batch/exportSkimData2017.sh --data -n 50 -f /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17
python makeSkimInput.py -r input/dict_skim_data_2018.py -w input/input_sets_skim_data_2018.txt -e batch/exportSkimData2018.sh --data -n 50 -f /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17
```
Note: this script uses the python file lists in [TreeMaker/Production/python](https://github.com/TreeMaker/TreeMaker/tree/Run2/Production/python) to determine the number of files to chain together for each sample.

To submit jobs to Condor (add the flag `-k` to reuse the existing CMSSW tarball):
```
cd batch
./SKsub.sh -t MC,Signal,Data -y 2016,2017,2018
./SKsub.sh -t Fast -y 2016,2017,2018
```
Note: the command with the argument `-t Fast` should only be run after the scanning step, below, is completed.

After the skims finish, some may need to be hadded (split or extended samples):
```
./hadd_skims.sh -r
```

<a name="cutflow"></a>Skimmed ROOT files include a histogram called "cutflow" which contains the raw number of events passing each selector in the selection, in order. To print a cutflow table from a skimmed ROOT file:
```
root -b -l -q 'KCutflowDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17/tree_signal/tree_T1tttt_1500_100.root")'
```
Additional arguments can be added to enable printing statistical errors and to change the number of significant figures printed.

## Scanning

The SUSY FastSim signal samples contain multiple model points per file. Before skimming, these samples should be scanned to separate each model point into a separate file.

To run the scanner interactively:
```
root -b -q -l 'KScanDriver.C+("T1tttt_MC2017_block0","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17",{"input/input_scan.txt"},{})'
```

To make the input lists (.txt and .sh) of FastSim samples automatically:
```
python makeScanInput.py -d Summer16v3Fast -r input/dict_scan_2016.py -w input/input_sets_scan_2016.txt -e batch/exportScan2016.sh -y 2016 -n 10
python makeScanInput.py -d Fall17Fast -r input/dict_scan_2017.py -w input/input_sets_scan_2017.txt -e batch/exportScan2017.sh -y 2017 -n 10
python makeScanInput.py -d Autumn18Fast -r input/dict_scan_2018.py -w input/input_sets_scan_2018.txt -e batch/exportScan2018.sh -y 2018 -n 10
```
The last argument splits the scan input list into multiple blocks (each containing `n` ntuple files) for batch submission.  
Note: as above, this script uses the python file lists in [TreeMaker/Production/python](https://github.com/TreeMaker/TreeMaker/tree/Run2/Production/python).

To submit jobs to Condor:
```
cd batch
./SCsub.sh -y 2016,2017,2018
```

After the jobs finish, the split output files should be combined (in batch mode due to the typically large number of hadd operations necessary):
```
./HSsub.sh -g "_block[0-9]*_MC2016" -S "MC2016_fast" -r
./HSsub.sh -g "_block[0-9]*_MC2017" -S "MC2017_fast" -r
./HSsub.sh -g "_block[0-9]*_MC2018" -S "MC2018_fast" -r
```

To make the input lists of model points automatically for skimming, plotting, and datacards, after the scan jobs are finished and combined:
```
python makeFastInput.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17/scan/ -m MC2016 -s input/input_sets_skim_fast_2016.txt -c input/input_sets_DC_fast_2016.txt -e batch/exportSkimFast2016.sh
python makeFastInput.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17/scan/ -m MC2017 -s input/input_sets_skim_fast_2017.txt -c input/input_sets_DC_fast_2017.txt -e batch/exportSkimFast2017.sh
python makeFastInput.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17/scan/ -m MC2018 -y MC2018 -s input/input_sets_skim_fast_2018.txt -c input/input_sets_DC_fast_2018.txt -e batch/exportSkimFast2018.sh
python makeFastInput.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17/scan/ -m MC2018 -y MC2018HEM -s input/input_sets_skim_fast_2018HEM.txt -c input/input_sets_DC_fast_2018HEM.txt -e batch/exportSkimFast2018HEM.sh
```

<a name="combined"></a>A separate script is available to create a "combined" model by adding together multiple signal models with different weights.
This script creates input lists for plotting and datacards (the existing skims are re-used).
```
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17/scan/ -s T1tbtb,T1tbbb,T1bbbb -w 0.25,0.5,0.25 -x T1tbtbT1tbbbT1bbbb
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV17/scan/ -s T1tbtb,T1tbtt,T1tttt -w 0.25,0.5,0.25 -x T1tbtbT1tbttT1tttt
```
If weights are not explicitly specified (using the `-w` option), each sample is weighted equally. The weights are always normalized so sum(weights) = 1.

## Plotting

To plot yields vs. RA2 bin (where the binning can be defined in the input file(s)) in the signal region and save the plot as an image:
```
root -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17/tree_signal",{"input/input_RA2bin.txt"},{},1)'
```
Omitting the last argument will display the plot without saving it.

## Datacard creation

To save the histogram for data to a ROOT file for datacard creation:
```
root -b -l -q 'MakeAllDCsyst.C+("data_2016","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17",{"input/input_DC_config_RA2data.txt"})'
```

To save the individual histograms for a FullSim signal process to a ROOT file for the signal region:
```
root -b -l -q 'MakeAllDCsyst.C+("T1tttt_1500_100","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17",{"input/input_DC_config_RA2full.txt"})'
```

To check specific systematics or variations, there are extra arguments. E.g., to check the ISR uncertainty and JEC variations:
```
root -b -l -q 'MakeAllDCsyst.C+("T1tttt_1500_100","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17",{"input/input_DC_config_RA2full.txt"},{},"","nominal,isruncUp,isruncDown","JECup,JECdown")'
```

Because of the large number of samples (especially the FastSim signal samples), batch mode should be used to run over them:
```
cd batch
./DCsub.sh -t Data -y Run2
./DCsub.sh -t Signal -y 2016,2017
./DCsub.sh -t Fast -y 2016,2017,2018,2018HEM
```

### Signal systematics

This driver produces two files for each mass point of each model:
one containing the nominal signal region histogram and all variations,
and another with the relative effect per bin of each systematic.
This driver also produces trees which store the effect of each systematic on the yield. These trees can be combined and analyzed:
```
cd batch
./HSsub.sh -c block,part,fast -d /store/user/pedrok/SUSY2015/Analysis/Datacards/Run2ProductionV17 -r
cd ..
python summarizeSyst.py -d /store/user/pedrok/SUSY2015/Analysis/Datacards/Run2ProductionV17
```

Currently available uncertainties:
* pileup reweighting
* scale weights (renormalization/factorization)
* initial state radiation
* trigger efficiency
* jet energy corrections
* jet energy resolution
* jet ID (fastsim only)
* isolated track efficiency (leptonic decays only)
* luminosity
* b-tagging scale factors
* mistagging scale factors
* b-tagging correction factors (fastsim only)
* c-tagging correction factors (fastsim only)
* mistagging correction factors (fastsim only)
* MHT modeling (fastsim only)
* MC statistics
* pileup acceptance (not used)
* L1 prefiring correction
* HEM veto

## Semi-visible jet commands

To remake the list of input files for semi-visible jet samples:
```
python makeSkimInput.py -r input/dict_skim_svj_bkg_2016.py -w input/input_sets_skim_svj_bkg_2016.txt -e batch/exportSkimSVJBkg2016.sh -n 50
python makeSkimInput.py -r input/dict_skim_svj_bkg_2017.py -w input/input_sets_skim_svj_bkg_2017.txt -e batch/exportSkimSVJBkg2017.sh -n 50
python makeSkimInput.py -r input/dict_skim_svj_bkg_2018.py -w input/input_sets_skim_svj_bkg_2018.txt -e batch/exportSkimSVJBkg2018.sh -n 50
python makeSkimInput.py -r input/dict_skim_svj_2017.py -w input/input_sets_skim_svj_2017.txt -e batch/exportSkimSVJ2017.sh -n 50 -N 200
```

To submit jobs to Condor:
```
cd batch
./SKsub_svj.sh -t SVJ -y 2017 -s
./SKsub_svj.sh -t SVJBkg,Data -y 2016,2017,2018
(wait for jobs to finish)
./hadd_skims.sh -d /store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims -n 1 -r
```

To make the list of scanning input files:
```
python makeScanInput.py -i SVJ -d Summer16v3 -r input/dict_scan_svj_2016.py -w input/input_sets_scan_svj_2016.txt -e batch/exportScanSVJ2016.sh -y 2016 -n 10
python makeScanInput.py -i SVJ -d Fall17 -r input/dict_scan_svj_2017.py -w input/input_sets_scan_svj_2017.txt -e batch/exportScanSVJ2017.sh -y 2017 -n 10
python makeScanInput.py -i SVJ -d Autumn18 -r input/dict_scan_svj_2018.py -w input/input_sets_scan_svj_2018.txt -e batch/exportScanSVJ2018.sh -y 2018 -n 10
```

To submit scan jobs to Condor:
```
cd batch
./SCsub_svj.sh -y 2016,2017,2018
(wait for jobs to finish)
for i in MC2016 MC2017 MC2018; do ./HSsub.sh -d /store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/scan -g "_block[0-9]*_${i}" -S "${i}_scan" -v; done
```

To make the input lists and skim the signal scans:
```
python makeFastInput.py -f -p 4 -d /store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/scan/ -m MC2016 -s input/input_sets_skim_svj_scan_2016.txt -c input/input_sets_DC_svj_scan_2016.txt -e batch/exportSkimSVJScan2016.sh
python makeFastInput.py -f -p 4 -d /store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/scan/ -m MC2017 -s input/input_sets_skim_svj_scan_2017.txt -c input/input_sets_DC_svj_scan_2017.txt -e batch/exportSkimSVJScan2017.sh
python makeFastInput.py -f -p 4 -d /store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/scan/ -m MC2018 -s input/input_sets_skim_svj_scan_2018.txt -c input/input_sets_DC_svj_scan_2018.txt -e batch/exportSkimSVJScan2018.sh
python makeFastInput.py -f -p 4 -d /store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/scan/ -m MC2018 -y MC2018PRE -s input/input_sets_skim_svj_scan_2018PRE.txt -c input/input_sets_DC_svj_scan_2018PRE.txt -e batch/exportSkimSVJScan2018PRE.sh
python makeFastInput.py -f -p 4 -d /store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/scan/ -m MC2018 -y MC2018POST -s input/input_sets_skim_svj_scan_2018POST.txt -c input/input_sets_DC_svj_scan_2018POST.txt -e batch/exportSkimSVJScan2018POST.sh
./batch/makeExportDCSVJScan.sh
cd batch
./SKsub_svj.sh -s -t SVJScan -y 2016,2017,2018
```

To make histograms, and then plots and ROC curves (including flattening pT spectra):
```
cd batch
./HPsub_svj_roc.sh -q jetpt -t SVJ,SVJBkg -y 2016,2017,2018
./haddEOS.sh ...
cd ..
./makePlotsSVJ.sh
```

To make BDT training ntuples:
```
cd batch
./SKsub_svj_train.sh -t SVJ,SVJQCD,SVJTTbar -y 2016,2017,2018
```

To make cutflow plots:
```
root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijethad",{"input/input_svj_cutflow_had.txt","input/input_svj_cutflow_sets_sig.txt"},{},1)'
root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijethad",{"input/input_svj_cutflow_had.txt","input/input_svj_cutflow_sets_qcd.txt"},{},1)'
root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijethad",{"input/input_svj_cutflow_had.txt","input/input_svj_cutflow_sets_ttbar.txt"},{},1)'
root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijethad",{"input/input_svj_cutflow_had.txt","input/input_svj_cutflow_sets_wjets.txt"},{},1)'
root -b -l -q 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijethad",{"input/input_svj_cutflow_had.txt","input/input_svj_cutflow_sets_zinv.txt"},{},1)'
```

To create histograms for datacards and do post-processing:
```
cd batch
./DCsub_svj.sh -t SVJData,SVJBkg -y Run2 -v trig6/sigfull
(wait for jobs to finish)
../finalizeDatacardsSVJ.sh -d /store/user/pedrok/SVJ2017/Datacards/trig6/sigfull -k
./DCsub_svj.sh -t SVJScan -y Run2 -v trig6/sigfull
```

Signal systematics studies:
```
cd batch
./HSsub.sh -c block,scan -d /store/user/pedrok/SVJ2017/Datacards/trig6/sigfull -S scan -k -n 1
cd ..
python summarizeSyst.py -d /store/user/pedrok/SVJ2017/Datacards/trig6/sigfull -c "mDark==20.&&rinv==0.3&&alpha==-2."
```
