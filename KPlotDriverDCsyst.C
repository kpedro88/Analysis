#include "KCode/KPlotManager.h"

using namespace std;

//how to run:
//root -l 'KPlotDriverDCsyst.C+("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV3/tree_signal","input/input_RA2bin_DC_syst.txt","input/input_sets_DC_syst.txt","RA2bin_signal")'
void KPlotDriverDCsyst(string dir, string inFile, string setList, string oname, string syst_opt="", int syst_val=0, bool doPrint = false){
	KPlotManager k(inFile,dir);
	k.Parse(setList);
	k.SetPrint(doPrint);
	k.GetGlobalOpt()->Set<string>("rootfile",oname);
	if(syst_opt.size()>0) k.GetGlobalOpt()->Set<int>(syst_opt,syst_val);
	k.DrawPlots();
}
