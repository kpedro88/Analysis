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
//root -b -q -l 'KScanDriver.C+("input/input_scan.txt","T1bbbb_mGluino-1000-1025_mLSP-1to975-1000","root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Ntuples/RA2sync_Sept23_2015")'
void KScanDriver(string input="", string setname="", string indir=""){
	gErrorIgnoreLevel = kBreak;
	
	if(input.size()==0){
		cout << "Recompiled KScanDriver, exiting." << endl;
		return;
	}
	
	KScanManager k(input,setname,indir);
	k.Scan();
}