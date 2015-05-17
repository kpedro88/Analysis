#include "KCode/KPlotManager.h"

using namespace std;

//how to run:
//root -l 'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/tree_main","input/input_RA2bin.txt")'
//test run:
//root -l 'KPlotDriver.C+("tree_test_main","input/input_RA2bin_test.txt")'
void KPlotDriver(string dir, string inFile, bool doPrint = false){
	KPlotManager k(inFile,dir);
	k.SetPrint(doPrint);
	k.DrawPlots();
}
