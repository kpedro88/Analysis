#include <TROOT.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "LQClass.C"

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
//root -b -q -l 'LQDriver.C++("")'
//to run interactively:
//root -b -q -l 'LQDriver.C+("LQ1000_tb_muTau","tree","main,zjets,anti-iso",0)'
void LQDriver(string infile, string treedir="tree",string selType="",bool doBatch=0){
	//gROOT->ProcessLine(".L LQClass.C+");

	//find correct mother ID
	int mother = -1;
	if(infile[0]=='Z' || infile[1]=='Z') mother = 23; //Zjets, ZZ, WZ -> Z
	else if(infile[0]=='W' || infile[0]=='T') mother = 24; //Wjets, WW, ttbar or single t -> W
	else if(infile[0]=='L') mother = 42; //LQ
	else if(infile[0]=='r') mother = 1000006; //RPV stop
	
	//open input file, get tree and nevents histo
	string fpre = "";
	if(infile.substr(0,7)!="rpvstop") fpre = "skim_";
	string fname = "~/hadoop/LQ2012-v4/" + fpre + infile + ".root";
	TFile *file;
	file = TFile::Open(fname.c_str());
	if (!file || !file->IsOpen()) {
		cout << "The file " << fname << " does not exist or cannot be opened." << endl;
		return;
	}
	TH1* nEventProc = 0;
	TTree* tree = 0;
	TDirectory* td = 0;
	file->GetObject("rootTupleTree",td);
	if(td) { td->GetObject("tree",tree); td->GetObject("nEventProc",nEventProc); }
	else { file->GetObject("tree",tree); file->GetObject("nEventProc",nEventProc); }

	//create LQClass
	LQClass q(tree,nEventProc,mother);
	q.SetBatch(doBatch);
	q.SetInFile(infile);
	
	//process selection types - comma-separated input
	vector<string> fields;
	process(selType,',',fields);
	
	if(fields.size()==0) {
		cout << "No selection types requested! No trees will be created." << endl;
		return;
	}
	
	for(unsigned f = 0; f < fields.size(); f++){
		//make sure the folder exists
		string treefolder = treedir + "_" + fields[f];
		string syscmd = "mkdir -p " + treefolder;
		system(syscmd.c_str());
		
		//add selection type and output filename to LQClass
		string oname = treefolder + "/tree_" + infile + ".root";
		q.AddSelType(fields[f],oname);
	}
	
	q.Loop();
}