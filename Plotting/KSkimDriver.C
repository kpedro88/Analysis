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
//root -b -q -l 'KSkimDriver.C+("input_selection.txt","LQ500_tb","main,zjets,anti-iso,main_TauESUp","/uscms_data/d3/pedrok/LQ2012/ntuple","/uscms_data/d3/pedrok/LQ2012/tree_test")'
void KSkimDriver(string input="", string setname="", string selTypes="", string indir="", string outdir="tree"){
	KSkimManager k(input,setname,selTypes,indir,outdir);
	k.Skim();
}