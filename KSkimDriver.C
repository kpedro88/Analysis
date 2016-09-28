//custom headers
#include "KCode/KSkimManager.h"

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
//root -b -q -l 'KSkimDriver.C++()'
//to run interactively:
//root -b -q -l 'KSkimDriver.C+("T1tttt_1500_100","signal","root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV10",{"input/input_selection.txt"},{},"test/tree")'
void KSkimDriver(string setname="", string selTypes="", string indir="", vector<string> input={}, vector<string> extras={}, string outdir="tree"){
	gErrorIgnoreLevel = kBreak;
	
	if(input.size()==0 && extras.size()==0){
		cout << "Recompiled KSkimDriver, exiting." << endl;
		return;
	}
	
	KSkimManager k(setname,selTypes,indir,input,extras,outdir);
	k.Skim();
}