#ifndef KSCANMANAGER_H
#define KSCANMANAGER_H

//custom headers
#include "KParser.h"
#include "KManager.h"
#include "KSet.h"
#include "KScanner.h"

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
class KScanManager : public KManager {
	public:
		//constructor
		KScanManager() : KManager(), setname(""), MyScanner(0), MyBase(0) {}
		KScanManager(string setname_, string indir_, vector<string> input_, vector<string> direct_) : 
			KManager(indir_), setname(setname_), MyScanner(0), MyBase(0)
		{
			//parse most initializations based on text input
			Initialize(input_,direct_);
			
			//safety checks
			if(!parsed) return;
			if(!MyBase){
				stringstream ss;
				KParser::printvec(input_,ss,",");
				throw runtime_error("set "+setname+" was not found in "+ss.str()+"!");
				parsed = false;
				return;
			}
			if(MyBase->GetTree()==NULL) { parsed = false; return; }
		}
		//destructor
		virtual ~KScanManager() {}
		void processSet(string line){
			//tab separated input
			vector<string> fields;
			KParser::process(line,'\t',fields);
			//only process the set of interest
			if(fields.size()>2 && fields[2]==setname) {
				//name check
				vector<string> outprelist;
				string outpre = "";
				if(globalOpt->Get("outprelist",outprelist)){
					for(unsigned s = 0; s < outprelist.size(); ++s){
						if(setname.find(outprelist[s])!=string::npos){
							outpre = outprelist[s];
							break;
						}
					}
					if(outpre.size()>0) globalOpt->Set<string>("outpre",outpre);
				}
				MyBase = KParser::processBase(line,globalOpt);
				MyScanner = static_cast<KScanner*>(MyBase->GetLooper());
			}
		}
		//where the magic happens
		void Scan(){
			if(!parsed) return;
			
			//scanner does the rest
			MyScanner->Loop();
			
			//final stuff
			MyScanner->CloseFile();
		}
		
	private:
		//member variables
		string setname;
		bool doClone;
		KScanner* MyScanner;
		KBase* MyBase;
};

#endif
