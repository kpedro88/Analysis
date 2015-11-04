#include "KCode/KPlotManager.h"

using namespace std;

//how to run:
//root -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/tree_signal","input/input_RA2bin.txt")'
void KPlotDriver(string dir="", string inFile="", bool doPrint = false){
	if(inFile.size()==0){
		cout << "Recompiled KPlotDriver, exiting." << endl;
		return;
	}
	
	KPlotManager k(inFile,dir);
	k.SetPrint(doPrint);
	k.DrawPlots();
}
