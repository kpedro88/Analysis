#include "KCode/KPlotManager.h"

using namespace std;

//how to run:
//root -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/tree_signal",{"input/input_RA2bin.txt"})'
void KPlotDriver(string indir="", vector<string> input={}, vector<string> extras={}, bool doPrint=false){
	if(input.size()==0 && extras.size()==0){
		cout << "Recompiled KPlotDriver, exiting." << endl;
		return;
	}
	
	KPlotManager k(indir,input,extras);
	k.SetPrint(doPrint);
	k.DrawPlots();
}
