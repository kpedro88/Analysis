//custom headers
#include "KCode/KScanManager.h"

//ROOT headers
#include <TError.h>
#include <TROOT.h>

//STL headers
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

//to recompile:
//root -b -q -l 'KScanDriver.C++()'
//to run interactively:
//root -b -q -l 'KScanDriver.C+("T1bbbb_block0","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV10",{"input/input_scan.txt"},{})'
void KScanDriver(string setname="", string indir="", vector<string> input={}, vector<string> extras={}){
	gErrorIgnoreLevel = kBreak;
	
	if(input.size()==0 && extras.size()==0){
		cout << "Recompiled KScanDriver, exiting." << endl;
		return;
	}
	
	KScanManager k(setname,indir,input,extras);
	k.Scan();
}