//custom headers
#include "KPlotDriverDCsyst.C"

//STL headers
#include <string>
#include <cstdlib>
#include <iostream>

#define nSyst 2

using namespace std;

//run in batch mode:
//root -b -l -q MakeAllDCsyst.C+
void MakeAllDCsyst(string indir="root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV2/"){
	if(indir[indir.size()-1] != '/') indir = indir + "/";
	string inpre = "tree_";
	string outdir = "datacards/";
	string outpre = "RA2bin_";
	string input = "input/input_RA2bin_DC_syst.txt";
	string region = "signal";
	string systs[nSyst] = {"btagSFunc","mistagSFunc"};
	
	//first do the nominal version
	KPlotDriverDCsyst(indir+inpre+region,input,outdir+outpre+region);
	
	//now do the variations
	for(unsigned i = 0; i < nSyst; ++i){
		cout << systs[i] << endl;
		//up
		KPlotDriverDCsyst(indir+inpre+region,input,outdir+outpre+region+"_"+systs[i]+"Up",systs[i],1);
		//down
		KPlotDriverDCsyst(indir+inpre+region,input,outdir+outpre+region+"_"+systs[i]+"Down",systs[i],-1);
	}
}