//custom headers
#include "KPlotDriverDC.C"

//STL headers
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

//run in batch mode:
//root -b -l -q MakeAllDCdata.C+
void MakeAllDCdata(string indir="", string region="", string outdir="datacards_data/"){
	if(indir.size()==0 || region.size()==0){
		cout << "Recompiled MakeAllDCdata, exiting." << endl;
		return;
	}
	
	if(indir[indir.size()-1] != '/') indir = indir + "/";
	string inpre = "tree_";
	string outpre = "RA2bin_";
	string input = "input/input_RA2bin_DC_data.txt";
	
	//check for directory
	system(("mkdir -p "+outdir).c_str());
	
	cout << region << endl;
	KPlotDriverDC(indir+inpre+region,input,outdir+outpre+region);
}