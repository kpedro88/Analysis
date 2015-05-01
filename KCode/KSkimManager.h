#ifndef KSKIMMANAGER_H
#define KSKIMMANAGER_H

//custom headers
#include "KManager.h"
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

//----------------------------------------------------------
//class to manage all selections and variations for skimming
class KSkimManager : public KManager {
	public:
		//constructor
		KSkimManager() : KManager(), setname(""), seltypes(""), outdir(""), skimmer(0), MyBase(0) {}
		KSkimManager(string input_, string setname_, string seltypes_, string indir_, string outdir_) : 
			KManager(input_, indir_), setname(setname_), seltypes(seltypes_), outdir(outdir_), skimmer(0), MyBase(0)
		{
			//initialize iterators
			curr_sel = allSelections.GetTable().end();
			curr_var = allVariations.GetTable().end();
			//parse most initializations based on text input
			Initialize(input_);
			
			//initialize skimmer after parsing
			if(!parsed) return;
			int mother = -1;
			MyBase->GetLocalOpt()->Get("mother",mother);
			globalOpt->Set("mother",mother);
			if(MyBase->GetTree()==NULL) { parsed = false; return; }//safety check
			skimmer = new KSkimmer(MyBase->GetTree(),MyBase->GetNEventHist(),globalOpt);
		}
		//destructor
		virtual ~KSkimManager() {}
		void processSet(string line){
			//tab separated input
			vector<string> fields;
			KParser::process(line,'\t',fields);
			//only process the set of interest
			if(fields.size()>2 && fields[2]==setname) {
				MyBase = KParser::processBase(line,globalOpt);
			}
		}
		//wait to construct variations until we know which are desired and with which selections to use them
		//for now, just store the lines for each defined variation in a vector, and construct variators with them later
		void processVariation(string line){
			if(line[0]=='\t'){ //variator
				line.erase(0,1);
				KNamed* tmp = KParser::processNamed(line);
				if(curr_var==allVariations.GetTable().end()){
					cout << "Input error: no variation for variator:" << endl << line << endl << "Check the indents. This input will be skipped." << endl;
					return;
				}
				curr_var->second.push_back(tmp);
			}
			else { //variation
				curr_var = allVariations.Add(line,vector<KNamed*>());
			}
		}
		//wait to construct selections until we know which are desired and what systematic variations to use
		//for now, just store the lines for each defined selection in a vector, and construct selectors with them later
		void processSelection(string line){
			if(line[0]=='\t'){ //selector
				line.erase(0,1);
				KNamed* tmp = KParser::processNamed(line);
				if(curr_sel==allSelections.GetTable().end()){
					cout << "Input error: no selection for selector:" << endl << line << endl << "Check the indents. This input will be skipped." << endl;
					return;
				}
				curr_sel->second.push_back(tmp);
			}
			else { //selection
				curr_sel = allSelections.Add(line,vector<KNamed*>());
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
				KParser::process(fields[i],'_',sel_unc);
				string sel = sel_unc[0];
				string unc = "";
				if(sel_unc.size()>1) unc = sel_unc[1];
				
				//check in full list (selections may be repeated with different systematics, so never remove things from the full list)
				if(allSelections.Has(sel)){
					//first check for existence of variation if requested
					KVariation* vntmp = 0;
					if(unc.size()>0) {
						if(!allVariations.Has(unc)) {
							cout << "Input error: variation " << unc << " is not defined. This request will be ignored." << endl;
							continue;
						}
						vntmp = new KVariation(unc);
						
						//create variators for variation
						vector<KNamed*> variatorLines = allVariations.Get(unc);
						for(unsigned v = 0; v < variatorLines.size(); v++){
							KVariator* vrtmp = KParser::processVariator(variatorLines[v]);
							if(vrtmp) vntmp->AddVariator(vrtmp);
						}
						vntmp->SetSkimmer(skimmer); //also sets skimmer for variators
					}
					
					//create selection using full name (sel + unc)
					KSelection* sntmp = new KSelection(fields[i]);
					if(vntmp) sntmp->SetVariation(vntmp);
					
					//create selectors for selection
					vector<KNamed*> selectorLines = allSelections.Get(sel);
					for(unsigned s = 0; s < selectorLines.size(); s++){
						KSelector* srtmp = KParser::processSelector(selectorLines[s]);
						if(srtmp) sntmp->AddSelector(srtmp);
					}
					sntmp->SetSkimmer(skimmer); //also sets skimmer for selectors
					
					//setup output tree
					sntmp->MakeTree(outdir,MyBase->GetName(), (globalOpt->Get("doClone",false) ? skimmer->fChain : NULL));

					//add to list
					skimmer->AddSelection(sntmp);
				}
				else {
					cout << "Input error: selection " << sel << " is not defined. This request will be ignored." << endl;
					continue;
				}
			}
			
			//skimmer does the rest
			skimmer->Loop();
			
			//final stuff
			MyBase->CloseFile();
			//if(globalOpt->Get("doBatch",false)) sleep(30);
		}
		
	private:
		//member variables
		string setname, seltypes, outdir;
		bool doBatch, doClone;
		KSkimmer* skimmer;
		KBase* MyBase;
		KMap<vector<KNamed*> > allSelections, allVariations;
		map<string,vector<KNamed*> >::iterator curr_sel, curr_var;
};


#endif