#include <TROOT.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "KCode/KSkimManager.h"

using namespace std;

//generalization for processing a line
void process(string line, char delim, vector<string>& fields){
	stringstream ss(line);
	string field;
	while(getline(ss,field,delim)){
		fields.push_back(field);
	}
}

//to recompile:
//root -b -q -l 'KSkimDriver.C++()'
//to run interactively:
//root -b -q -l 'KSkimDriver.C+("input/input_selection.txt","T1tttt_1500_100","main","root://cmseos.fnal.gov//store/user/awhitbe1/photonTestingV2","/uscms_data/d3/pedrok/SUSY2015/crab/CMSSW_7_3_1_patch2/src/Analysis/tree_test")'
void KSkimDriver(string input="", string setname="", string selTypes="", string indir="", string outdir="tree"){
	if(input.size()==0){
		cout << "Recompiled KSkimDriver, exiting." << endl;
		return;
	}
	KSkimManager k(input,setname,selTypes,indir,outdir);
	k.Skim();
}