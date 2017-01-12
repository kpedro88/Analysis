//custom headers
#include "KPlotDriver.C"

//ROOT headers
#include <TError.h>

//STL headers
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

//recompile:
//root -b -l -q MakeAllDCsyst.C++
void MakeAllDCsyst(int mode=-1, string setname="", string indir="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV11", string systTypes="nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown", string varTypes="JECup,JECdown,JERup,JERdown"){
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
	//string inputQCD = "input/input_RA2bin_DC_QCD.txt";
	string setlist = "";
	string osuff = "";
	
	//process variaton types - comma-separated input, need to be run separately
	vector<string> vars;
	KParser::process(varTypes,',',vars);
	if(mode==1){
		outdir = "datacards_fast/";
		setlist = "input/fast/input_set_DC_"+setname+".txt";
		osuff = "_"+setname;
	}
	else {
		outdir = "datacards_syst/";
		setlist = "input/input_set_DC_"+setname+".txt";
		osuff = "_"+setname;
	}
	
	//check for directory
	if(outdir.size()>0) system(("mkdir -p "+outdir).c_str());
	
	//keep a list of root files
	vector<string> rootfiles;
	
	//do the simple systematics all at once
	rootfiles.push_back(outdir+outpre+region+osuff);
	KPlotDriver(indir+inpre+region,{input,setlist},{"OPTION","string:rootfile["+rootfiles.back()+"]","vstring:selections["+systTypes+"]"});
	
	//do the full variations separately
	//produce a selection for each variation on the fly, cloned from nominal
	for(auto& ivar : vars){
		//change region
		string region_ = region + "_"+ivar;
		rootfiles.push_back(outdir+outpre+region_+osuff);
		KPlotDriver(indir+inpre+region_,{input,setlist},{"OPTION","string:rootfile["+rootfiles.back()+"]","vstring:selections["+ivar+"]","SELECTION",ivar,"\tnominal"});
	}
	
	//hadd
	stringstream slist;
	copy(rootfiles.begin(),rootfiles.end(),ostream_iterator<string>(slist," "));
	string therootfile = outpre+osuff;
	string cmd = "hadd -f "+therootfile+" "+slist.str();
	system(cmd.c_str());
	
	//further processing
	/*
	TFile* infile = TFile::Open(therootfile.c_str());
	
	
	string thenewfile = outpre+"proc"+osuff;
	TFile* outfile = TFile::Open(thenewfile.c_str(),"RECREATE");
	*/
}