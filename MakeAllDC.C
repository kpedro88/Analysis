//custom headers
#include "KPlotDriverDC.C"

//STL headers
#include <string>
#include <cstdlib>
#include <iostream>

#define nRegions 5

using namespace std;

//run in batch mode:
//root -b -l -q MakeAllDC.C+
void MakeAllDC(){
	string indir = "root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/";
	string inpre = "tree_";
	string input = "input/input_RA2bin_DC.txt";
	string outdir = "datacards/";
	string outpre = "RA2bin_";
	string regions[nRegions] = {"signal","LDP","SLe","SLm","GJet_NoPhotonVars"};
	
	for(unsigned i = 0; i < nRegions; ++i){
		cout << regions[i] << endl;
		KPlotDriverDC(indir+inpre+regions[i],input,outdir+outpre+regions[i]);
	}
	
	//add up SL regions
	system(("hadd -f "+outdir+outpre+"SL.root "+outdir+outpre+"SLe.root "+outdir+outpre+"SLm.root").c_str());
}