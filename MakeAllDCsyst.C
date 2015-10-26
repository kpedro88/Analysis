//custom headers
#include "KPlotDriverDCsyst.C"

//ROOT headers
#include <TError.h>

//STL headers
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

//recompile:
//root -b -l -q MakeAllDCsyst.C++
void MakeAllDCsyst(int mode=-1, string indir="root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV3/", int part=-1){
	gErrorIgnoreLevel = kBreak;
	
	if(mode==-1){
		cout << "Recompiled MakeAllDCsyst, exiting." << endl;
		return;
	}
	
	if(indir[indir.size()-1] != '/') indir = indir + "/";
	string inpre = "tree_";
	string region = "signal";
	string outpre = "RA2bin_";
	string outdir = "";
	string input = "input/input_RA2bin_DC_syst.txt";
	string setlist = "";
	string osuff = "";
	unsigned nSyst = 0;
	vector<string> systs;
	if(mode==1){
		if(part==-1){
			outdir = "datacards_fast/";
			setlist = "input/input_sets_DC_fast.txt";
		}
		else { //batch mode
			stringstream ss;
			ss << "input/fast/input_sets_DC_fast_" << part << ".txt";
			setlist = ss.str();
			ss.str(string());
			ss << "_part" << part;
			osuff = ss.str();
		}
		nSyst = 8;
		string systmp[] = {"puunc","pdfunc","trigStatUnc","trigSystUnc","btagSFunc","mistagSFunc","btagCFunc","ctagCFunc","mistagCFunc"};
		systs = vector<string>(systmp,systmp+nSyst);
	}
	else {
		outdir = "datacards/";
		setlist = "input/input_sets_DC_syst.txt";
		nSyst = 5;
		string systmp[] = {"puunc","pdfunc","trigStatUnc","trigSystUnc","btagSFunc","mistagSFunc"};
		systs = vector<string>(systmp,systmp+nSyst);
	}
	
	//check for directory
	if(outdir.size()>0) system(("mkdir -p "+outdir).c_str());
	
	//first do the nominal version
	cout << "nominal" << endl;
	KPlotDriverDCsyst(indir+inpre+region,input,setlist,outdir+outpre+region+osuff);
	
	//now do the variations
	for(unsigned i = 0; i < nSyst; ++i){
		//up
		cout << systs[i] << " up" << endl;
		KPlotDriverDCsyst(indir+inpre+region,input,setlist,outdir+outpre+region+"_"+systs[i]+"Up"+osuff,systs[i],1);
		//down
		cout << systs[i] << " down" << endl;
		KPlotDriverDCsyst(indir+inpre+region,input,setlist,outdir+outpre+region+"_"+systs[i]+"Down"+osuff,systs[i],-1);
	}
	
	if(outdir.size()>0){
		string zipname = outdir;
		if(zipname[zipname.size()-1] == '/') zipname.erase(zipname.size()-1,1);
		zipname += ".zip";
		system(("zip -r "+zipname+" "+outdir).c_str());
	}
}