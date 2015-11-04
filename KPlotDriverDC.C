#include "KCode/KPlotManager.h"

using namespace std;

//how to run:
//root -l 'KPlotDriverDC.C+("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/tree_signal","input/input_RA2bin_DC.txt","RA2bin_signal")'
void KPlotDriverDC(string dir="", string inFile="", string oname="", bool doPrint = false){
	if(inFile.size()==0){
		cout << "Recompiled KPlotDriverDC, exiting." << endl;
		return;
	}
	
	KPlotManager k(inFile,dir);
	k.SetPrint(doPrint);
	k.GetGlobalOpt()->Set<string>("rootfile",oname);
	k.DrawPlots();
}
