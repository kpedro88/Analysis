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
//root -b -q -l 'KSkimDriver.C+("input/input_selection.txt","T1tttt_1500_100","signal","root://cmseos.fnal.gov//store/user/awhitbe1/PHYS14productionV12","tree_test")'
void KSkimDriver(string input="", string setname="", string selTypes="", string indir="", string outdir="tree"){
	gErrorIgnoreLevel = kBreak;
	
	if(input.size()==0){
		cout << "Recompiled KSkimDriver, exiting." << endl;
		return;
	}
	
	KSkimManager k(input,setname,selTypes,indir,outdir);
	k.Skim();
}