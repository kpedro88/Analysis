#ifndef KSKIMMANAGER_H
#define KSKIMMANAGER_H

//custom headers
#include "KParser.h"
#include "KSet.h"
#include "KVariators.h"
#include "KSelectors.h"
#include "KSkimmer.h"

//ROOT headers
#include <TROOT.h>

//STL headers
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>
#include <unistd.h> //for sleep

using namespace std;

//------------------------------------------
//class to manage all objects and draw plots
class KSkimManager {
	public:
		//constructor
		KSkimManager() : input(""), setname(""), seltypes(""), indir(""), outdir(""), skimmer(0), MyBase(0), curr_var(0), parsed(false) {
			//initialize iterator
			curr_sel = allSelections.end();
			//must always have an option map
			globalOpt = new OptionMap();
		}
		KSkimManager(string input_, string setname_, string seltypes_, string indir_, string outdir_) : 
			input(input_), setname(setname_), seltypes(seltypes_), indir(indir_), outdir(outdir_), skimmer(0), MyBase(0), curr_var(0), parsed(false) 
		{
			//initialize iterator
			curr_sel = allSelections.end();
			//parse most initializations based on text input
			globalOpt = new OptionMap();
			//store treedir in global options
			globalOpt->Set("treedir",indir);
			parsed = Parse(input);
			int prcsn;
			if(globalOpt->Get("yieldprecision",prcsn)) cout << fixed << setprecision(prcsn);
			
			//initialize skimmer after parsing (with reference to selection list)
			int mother = -1;
			MyBase->GetLocalOpt()->Get("mother",mother);
			globalOpt->Set("mother",mother);
			skimmer = new KSkimmer(MyBase->GetTree(),MyBase->GetNEventHist(),theSelections,globalOpt);
		}
		//destructor
		virtual ~KSkimManager() {}
		//parse input file
		bool Parse(string input){
			bool parsed_ = true;
			string intype;
			string line;
			ifstream infile(input.c_str());
			if(infile.is_open()){
				while(getline(infile,line)){
					//skip commented lines
					if(line[0]=='#') continue;
					//skip blank lines
					if(line.size()<2) continue;
					
					//check for carriage returns (not allowed)
					if(line[line.size()-1]=='\r') {
						cout << "Carriage return detected. Please run:" << endl;
						cout << "dos2unix " << input << endl;
						cout << "and then try again." << endl;
						return false;
					}
					
					//check for input type
					if(line.compare(0,6,"OPTION")==0) { intype = "OPTION"; continue; }
					else if(line.compare(0,3,"SET")==0) { intype = "SET"; continue; }
					else if(line.compare(0,10,"VARIATION")==0) { intype = "VARIATION"; continue; }
					else if(line.compare(0,9,"SELECTION")==0) { intype = "SELECTION"; continue; }
					else if(line.compare(0,5,"INPUT")==0) {
						//another input file to parse on next line
						getline(infile,line);
						parsed_ &= Parse(line);
						continue;
					}
					
					//otherwise, process line according to input type
					if(intype=="OPTION") KParser::processOption(line,globalOpt);
					else if(intype=="SET") processSet(line);
					else if(intype=="VARIATION") processVariation(line);
					else if(intype=="SELECTION") processSelection(line);
				}
				parsed_ &= true;
			}
			else {
				cout << "Input error: could not open input file \"" << input << "\"." << endl;
				parsed_ &= false;
			}
			return parsed_;
		}
		void processSet(string line){
			//tab separated input
			vector<string> fields;
			process(line,'\t',fields);
			//only process the set of interest
			if(fields.size()>2 && fields[2]==setname) {
				MyBase = KParser::processBase(line,globalOpt);
			}
		}
		void processVariation(string line){
			if(line[0]=='\t'){ //variator
				line.erase(0,1);
				KNamed tmp = KParser::processNamed(line);
				
				KVariator* vtmp = KParser::processVariator(tmp);
				
				if(vtmp) curr_var->AddVariator(tmp);
			}
			else { //variation
				curr_var = new KVariation(line);
				allVariations.Add(line,curr_var);
			}
		}
		//wait to construct selections until we know which are desired and what systematic variations to use
		//for now, just store the lines for each defined selection in a vector, and construct selectors with them later
		void processSelection(string line){
			if(line[0]=='\t'){ //selector
				line.erase(0,1);
				KNamed tmp = KParser::processNamed(line);
				if(curr_sel==allSelections.end()){
					cout << "Input error: no selection for selector:" << endl << line << endl << "Check the indents. This input will be skipped."
					return;
				}
				curr_sel->second.push_back(tmp);
			}
			else { //selection
				curr_sel = allSelections.Add(line,vector<KNamed>());
			}
		}
		//where the magic happens
		void Skim(){
			if(!parsed) return;
			
			//process selection types - comma-separated input
			vector<string> fields;
			KParser::process(seltypes,',',fields);
			if(fields.size()==0) {
				cout << "Input error: no selection types requested! No trees will be created." << endl;
				return;
			}
			
			//construct requested selections
			for(unsigned i = 0; i < fields.size(); i++){
				//check for systematic
				vector<string> sel_unc;
				process(fields[i],'_',sel_unc);
				string sel = sel_unc[0];
				string unc = "";
				if(fields.size()>1) unc = sel_unc[1];
				
				//check in full list (selections may be repeated with different systematics, so never remove things from the full list)
				if(allSelections.Has(sel)){
					//first check for existence of variation if requested
					KVariation* utmp = 0;
					if(unc.size()>0) {
						utmp = allVariations.Get(unc);
						if(!utmp) {
							cout << "Input error: variation " << unc << " is not defined. This request will be ignored." << endl;
							continue;
						}
					}
					
					//create selection using full name (sel + unc)
					KSelection* sntmp = new KSelection(fields[i]);
					if(utmp) sntmp.SetVariation(utmp);
					
					//create selectors for selection
					vector<KNamed> selectorLines = allSelections.Get(sel);
					for(unsigned s = 0; s < selectorLines.size(); s++){
						KSelector* srtmp = KParser::processSelector(selectorLines[s]);
						if(srtmp) sntmp.AddSelector(srtmp);
					}
					sntmp->SetSkimmer(skimmer); //also sets skimmer for selectors
					
					//setup output tree
					sntmp->MakeTree(outdir,MyBase->GetFilename());

					//add to list
					theSelections.push_back(sntmp);
				}
				else {
					cout << "Input error: selection " << sel << " is not defined. This request will be ignored." << endl;
				}
			}
			
			//skimmer does the rest
			skimmer->Loop();
			
			//final stuff
			MyBase->CloseFile();
			//if(globalOpt->Get("doBatch",false)) sleep(30);
		}
		
		//accessors
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void ListOptions() {
			OMit it;
			for(it = globalOpt->GetTable().begin(); it != globalOpt->GetTable().end(); it++){
				cout << it->first /*<< ": " << it->second->value*/ << endl;
			}
		}
		
	private:
		//member variables
		string input, setname, seltypes, indir, outdir;
		bool doBatch, doClone;
		KSkimmer* skimmer;
		KBase* MyBase;
		OptionMap* globalOpt;
		KMap<string,vector<KNamed> > allSelections;
		vector<KSelection*> theSelections;
		map<string,vector<KNamed> >::iterator curr_sel;
		Variation allVariations;
		KVariation* curr_var;
		bool parsed;
};


#endif