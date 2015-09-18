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
			skimmer = new KSkimmer(MyBase->GetTree(),MyBase->GetNEventHist(),MyBase->GetNEventNegHist(),globalOpt);
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
				//make selection
				KSelection<KSkimmer>* sntmp = makeSelection<KSkimmer>(fields[i]);
				
				if(sntmp){
					//setup output tree
					sntmp->MakeTree(outdir,MyBase->GetName(), (globalOpt->Get("doClone",false) ? skimmer->fChain : NULL));

					//add to list
					skimmer->AddSelection(sntmp);
				}
				else {
					continue;
				}
			}
			
			//skimmer does the rest
			skimmer->Loop();
			
			//final stuff
			MyBase->CloseFile();
		}
		
	private:
		//member variables
		string setname, seltypes, outdir;
		bool doClone;
		KSkimmer* skimmer;
		KBase* MyBase;
};


#endif