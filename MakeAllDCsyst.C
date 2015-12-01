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
void MakeAllDCsyst(int mode=-1, string indir="root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV4/", string systTypes="nominal,QCD,puunc,pdfunc,scaleunc,isrunc,trigStatUnc,trigSystUnc,JEC,btagSFunc,ctagSFunc,mistagSFunc", string contamTypes="LDP,GJet_CleanVars,GJetLDP_CleanVars,GJet_CleanVarsGJ,GJetLDP_CleanVarsGJ", int part=-1){
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
	
	//process syst/contam types - comma-separated input
	vector<string> contams;
	KParser::process(contamTypes,',',contams);
	vector<string> systs;
	KParser::process(systTypes,',',systs);
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
	}
	else {
		outdir = "datacards/";
		setlist = "input/input_sets_DC_syst.txt";
	}
	
	//check for directory
	if(outdir.size()>0) system(("mkdir -p "+outdir).c_str());
	
	//first do the nominal version
	
	
	//do the variations
	for(unsigned i = 0; i < systs.size(); ++i){
		//special cases
		
		//the nominal version
		if(systs[i]=="nominal"){
			cout << "nominal" << endl;
			KPlotDriverDCsyst(indir+inpre+region,input,setlist,outdir+outpre+region+osuff);
			continue;
		}
		//220 bin version
		else if(systs[i]=="QCD"){
			cout << "220 bins" << endl;
			KPlotDriverDCsyst(indir+inpre+region,"input/input_RA2bin_DC_QCD.txt",setlist,outdir+outpre+region+"_QCD"+osuff);
			continue;
		}
		
		string ovar = "";
		//up
		cout << systs[i] << " up" << endl;
		if(systs[i]=="JEC") region = "signal_JECup";
		else ovar = "_"+systs[i]+"Up";
		KPlotDriverDCsyst(indir+inpre+region,input,setlist,outdir+outpre+region+ovar+osuff,systs[i],1);
		//down
		cout << systs[i] << " down" << endl;
		if(systs[i]=="JEC") region = "signal_JECdown";
		else ovar = "_"+systs[i]+"Down";
		KPlotDriverDCsyst(indir+inpre+region,input,setlist,outdir+outpre+region+ovar+osuff,systs[i],-1);
		
		//reset region if altered for jet syst
		if(systs[i]=="JEC") region = "signal";
	}
	
	//now do the contaminations
	for(unsigned i = 0; i < contams.size(); ++i){
		cout << contams[i] << endl;
		KPlotDriverDCsyst(indir+inpre+contams[i],input,setlist,outdir+outpre+contams[i]+osuff);
	}

}