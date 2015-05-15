#ifndef KSKIMMANAGER_H
#define KSKIMMANAGER_H

//custom headers
#include "KManager.h"
#include "KSet.h"
#include "KSkimmer.h"
#include "KSkimmerVariators.h"
#include "KSkimmerSelectors.h"

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
					vector<KNamed*> variatorLines;
					if(unc.size()>0) {
						if(!allVariations.Has(unc)) {
							cout << "Input error: variation " << unc << " is not defined. This request will be ignored." << endl;
							continue;
						}
						
						//get variators info
						variatorLines = allVariations.Get(unc);
					}
					
					//get selectors info
					vector<KNamed*> selectorLines = allSelections.Get(sel);
					
					//make selection
					KSelection<KSkimmer>* sntmp = makeSelection<KSkimmer>(sel,selectorLines,unc,variatorLines,skimmer);
					
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
};


#endif