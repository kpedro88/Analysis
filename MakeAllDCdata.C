//custom headers
#include "KPlotDriverDC.C"

//STL headers
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

//run in batch mode:
//root -b -l -q MakeAllDCdata.C+
void MakeAllDCdata(string indir="root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV2"){
	if(indir[indir.size()-1] != '/') indir = indir + "/";
	string inpre = "tree_";
	string outdir = "datacards_data/";
	string outpre = "RA2bin_";
	string input = "input/input_RA2bin_DC_data.txt";
	string region = "signal";
	
	//check for directory
	system(("mkdir -p "+outdir).c_str());
	
	cout << region << endl;
	KPlotDriverDC(indir+inpre+region,input,outdir+outpre+region);
}