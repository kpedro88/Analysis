//custom headers
#include "KPlotDriverDCsyst.C"

//STL headers
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

//run in batch mode:
//root -b -l -q MakeAllDCsyst.C+
void MakeAllDCsyst(bool fastsim=false, string indir="root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV2/"){
	if(indir[indir.size()-1] != '/') indir = indir + "/";
	string inpre = "tree_";
	string region = "signal";
	string outpre = "RA2bin_";
	string outdir = "";
	string input = "";
	int nSyst = 0;
	vector<string> systs;
	if(fastsim){
		outdir = "datacards_fast/";
		input = "input/input_RA2bin_DC_fast_syst.txt";
		nSyst = 5;
		string systmp[] = {"btagSFunc","mistagSFunc","btagCFunc","ctagCFunc","mistagCFunc"};
		systs = vector<string>(systmp,systmp+nSyst);
	}
	else {
		outdir = "datacards/";
		input = "input/input_RA2bin_DC_syst.txt";
		nSyst = 2;
		string systmp[] = {"btagSFunc","mistagSFunc"};
		systs = vector<string>(systmp,systmp+nSyst);
	}
	
	//check for directory
	system(("mkdir -p "+outdir).c_str());
	
	//first do the nominal version
	cout << "nominal" << endl;
	KPlotDriverDCsyst(indir+inpre+region,input,outdir+outpre+region);
	
	//now do the variations
	for(unsigned i = 0; i < nSyst; ++i){
		//up
		cout << systs[i] << " up" << endl;
		KPlotDriverDCsyst(indir+inpre+region,input,outdir+outpre+region+"_"+systs[i]+"Up",systs[i],1);
		//down
		cout << systs[i] << " down" << endl;
		KPlotDriverDCsyst(indir+inpre+region,input,outdir+outpre+region+"_"+systs[i]+"Down",systs[i],-1);
	}
	
	string zipname = outdir;
	if(zipname[zipname.size()-1] == '/') zipname.erase(zipname.size()-1,1);
	zipname += ".zip";
	system(("zip -r "+zipname+" "+outdir).c_str());
}