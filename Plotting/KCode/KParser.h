#ifndef KPARSER_H
#define KPARSER_H

//custom headers
#include "KManager.h"
#include "KHelper.h"

//ROOT headers
#include <TROOT.h>

//STL headers
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

//--------------------------------------------------
//class to parse input options for the manager
class KParser {
	public:
		//constructors
		KParser() : MyManager(0), parsed(false) {}
		KParser(KManager* MyManager_) : MyManager(MyManager_), parsed(false) {}
		//destructor
		virtual ~KParser() {}
		
		//parse input file
		bool Parse(){
			string intype;
			string line;
			ifstream infile(MyManager->input.c_str());
			if(infile.is_open()){
				while(getline(infile,line)){
					//skip commented lines
					if(line[0]=='#') continue;
					//skip blank lines
					if(line.size()<2) continue;
					
					//check for carriage returns (not allowed)
					if(line[line.size()-1]=='\r') {
						cout << "Carriage return detected. Please run:" << endl;
						cout << "dos2unix " << MyManager->input << endl;
						cout << "and then try again." << endl;
						return false;
					}
					
					//check for input type
					if(line.compare(0,6,"OPTION")==0) { intype = "OPTION"; continue; }
					else if(line.compare(0,3,"SET")==0) { intype = "SET"; continue; }
					else if(line.compare(0,7,"HISTO2D")==0) { intype = "HISTO2D"; continue; }
					else if(line.compare(0,5,"HISTO")==0) { intype = "HISTO"; continue; }
					
					//otherwise, process line according to input type
					if(intype=="OPTION") KText::processOption(line,MyManager->globalOpt);
					else if(intype=="SET") processSet(line);
					else if(intype=="HISTO2D") processHisto(line,2);
					else if(intype=="HISTO") processHisto(line,1);
				}
				parsed = true;
			}
			else {
				cout << "Input error: could not open input file \"" << MyManager->input << "\"." << endl;
				parsed = false;
			}
			return parsed;
		}
		void processSet(string line){
			//cout << line << endl;
			int indent = 0;
			while(line[0]=='\t'){
				line.erase(0,1);
				indent++;
			}
			
			//tab separated input
			vector<string> fields;
			KText::process(line,'\t',fields);
			
			//debug: print the contents of curr_sets
			/*map<int,KBase*>::iterator dit;
			for(dit = curr_sets.begin(); dit != curr_sets.end(); dit++){
				cout << dit->second->GetName() << ", ";
			}
			cout << endl;*/
			
			//check for necessary options
			if(fields.size()<3) {
				cout << "Input error: not enough fields in this line:" << endl << line << endl << "Check that all fields are tab-separated. This input will be ignored." << endl;
				return;
			}
			//universal variables
			string type = fields[0];
			string subtype = fields[1];
			string name = fields[2];
			
			//check corner cases for indent
			if(indent==0 && fields[0]=="base"){
				cout << "Input error: base must have a parent set! This input will be ignored." << endl;
				return;
			}
			else if(indent>0){
				map<int,KBase*>::iterator it = MyManager->curr_sets.find(indent-1);
				if(it == MyManager->curr_sets.end()){
					cout << "Input error: no parent can be identified for the set " << name << "! There may be too many indents, or the parent was not input properly. This input will be ignored." << endl;
					return;
				}
			}
			
			//reset entries in the map of current sets that have higher (or equal) indents vs. current line
			map<int,KBase*>::iterator it = MyManager->curr_sets.lower_bound(indent);
			MyManager->curr_sets.erase(it,MyManager->curr_sets.end());
			
			OptionMap* omap = new OptionMap(); //for local options
			//process local options before constructing objects
			for(int i = 3; i < fields.size(); i++){
				KText::processOption(fields[i],omap);
			}
			
			KBase* tmp = 0;
			if(type=="base"){
				if(subtype=="ext") tmp = new KBaseExt(name,omap,MyManager->globalOpt);
				else if(subtype=="data") tmp = new KBaseData(name,omap,MyManager->globalOpt); 
				else if(subtype=="mc") tmp = new KBaseMC(name,omap,MyManager->globalOpt);
			}
			else {
				//create object
				if(type=="hist" && subtype=="data") tmp = new KSetData(name,omap,MyManager->globalOpt);
				else if(type=="hist" && subtype=="mc") tmp = new KSetMC(name,omap,MyManager->globalOpt);
				else if(type=="stack" && subtype=="mc") tmp = new KSetMCStack(name,omap,MyManager->globalOpt);
			}
			
			//store and add to parent set
			if(tmp){
				MyManager->curr_sets[indent] = tmp;
				if(indent==0) MyManager->MySets.push_back(MyManager->curr_sets[indent]); //add top-level set to manager
				else {
					MyManager->curr_sets[indent-1]->AddChild(tmp);
					//cout << curr_sets[indent-1]->GetName() << "->AddChild(" << tmp->GetName() << ")" << endl;
				}
			}
			else {
				cout << "Input error: set type \"" << type << " " << subtype << "\" is not recognized. This input will be ignored." << endl;
				return;
			}
		}
		void processHisto(string line, int dim){
			//tab separated input
			vector<string> fields;
			KText::process(line,'\t',fields);
			
			string name = fields[0];
			OptionMap* omap = new OptionMap();
			
			//everything after the name is an option
			for(int i = 1; i < fields.size(); i++){
				KText::processOption(fields[i],omap);
			}
			
			//keep track of histo dimension
			omap->Set("dimension",dim);
			
			//store local plot options for later use
			MyManager->MyPlotOptions.Add(name,omap);
		}
		
	
	private:
		//member variables
		KManager* MyManager;
		bool parsed;

};

//implementation of KManager constructor
//note: dir="tree" default
KManager::KManager(string in, string dir) : input(in),treedir(dir),globalOpt(0),numer(0),denom(0),yieldref(0),doPrint(false),varbins(0),nbins(0),parsed(false) {
	//parser does most initializations based on text input
	globalOpt = new OptionMap();
	//store treedir in global options
	globalOpt->Set("treedir",treedir);
	MyParser = new KParser(this);
	parsed = MyParser->Parse();
	
	//final checks and initializations
	int prcsn;
	if(globalOpt->Get("yieldprecision",prcsn)) cout << fixed << setprecision(prcsn);
	MyRatio = new KSetRatio(NULL,globalOpt);
	if(globalOpt->Get("calcfaketau",false)) FakeTauEstimationInit();
	//store correction root files centrally
	//todo: make file location and histo name configurable
	if(globalOpt->Get("pucorr",true)) {
		TFile* pufile = new TFile("corrections/puWeightsLQ.root","READ"); //puWeights
		TH1F* puWeights = (TH1F*)pufile->Get("pileup");
		globalOpt->Set("puWeights",puWeights);
	}
	if(globalOpt->Get("mucorr",true)) {
		TFile* mufile = new TFile("corrections/muIDTight.root","READ"); //puWeights
		TH1F* muIDTight = (TH1F*)mufile->Get("muIDTight");
		globalOpt->Set("muIDTight",muIDTight);
	}
}

#endif