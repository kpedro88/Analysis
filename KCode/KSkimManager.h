#ifndef KSKIMMANAGER_H
#define KSKIMMANAGER_H

//custom headers
#include "KParser.h"
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
#include <exception>

using namespace std;

//----------------------------------------------------------
//class to manage all selections and variations for skimming
class KSkimManager : public KManager {
	public:
		//constructor
		KSkimManager() : KManager(), setname(""), seltypes(""), outdir(""), MySkimmer(0), MyBase(0) {}
		KSkimManager(string setname_, string seltypes_, string indir_, vector<string> input_, vector<string> direct_, string outdir_) : 
			KManager(indir_), setname(setname_), seltypes(seltypes_), outdir(outdir_), MySkimmer(0), MyNamedBase(0), MyBase(0)
		{
			//parse most initializations based on text input
			Initialize(input_,direct_);
			
			//safety checks
			if(!parsed) return;
			if(!MyNamedBase){
				stringstream ss;
				KParser::printvec(input_,ss,",");
				throw runtime_error("set "+setname+" was not found in "+ss.str()+"!");
				parsed = false;
				return;
			}
			MyBase = KParser::processBase(MyNamedBase,globalOpt);
			MySkimmer = static_cast<KSkimmer*>(MyBase->GetLooper());
			if(MyBase->GetTree()==NULL) { parsed = false; return; }
		}
		//destructor
		virtual ~KSkimManager() {}
		void processSet(string line){
			//tab separated input
			vector<string> fields;
			KParser::process(line,'\t',fields);
			//only process the set of interest
			if(fields.size()>2 && fields[2]==setname) {
				MyNamedBase = KParser::processNamed<3>(line);
			}
		}
		//where the magic happens
		void Skim(){
			if(!parsed) return;
		
			//process selection types - comma-separated input
			vector<string> fields;
			KParser::process(seltypes,',',fields);
			if(fields.size()==0) {
				throw runtime_error("no selection types requested! No trees can be created.");
			}
			
			//construct requested selections
			MySkimmer->MakeNEventSelection();
			for(unsigned i = 0; i < fields.size(); i++){
				//make selection
				KSelection* sntmp = makeSelection(fields[i]);
				
				if(sntmp){
					//add to list
					MySkimmer->AddSelection(sntmp);
					
					//setup output tree
					sntmp->MakeTree(outdir, MyBase->GetName(), (globalOpt->Get("doClone",false) ? MySkimmer->fChain : NULL));
				}
				else {
					continue;
				}
			}
			
			//skimmer does the rest
			MySkimmer->Loop();
			
			//final stuff
			MySkimmer->CloseFile();
		}
		
	private:
		//member variables
		string setname, seltypes, outdir;
		bool doClone;
		KSkimmer* MySkimmer;
		KNamedBase* MyNamedBase;
		KBase* MyBase;
};

#endif
