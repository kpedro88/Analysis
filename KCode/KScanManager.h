#ifndef KSCANMANAGER_H
#define KSCANMANAGER_H

//custom headers
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

using namespace std;

//----------------------------------------------------------
//class to manage all selections and variations for skimming
class KScanManager : public KManager {
	public:
		//constructor
		KScanManager() : KManager(), setname(""), scanner(0), MyBase(0) {}
		KScanManager(string input_, string setname_, string indir_) : 
			KManager(indir_), setname(setname_), scanner(0), MyBase(0)
		{
			//parse most initializations based on text input
			Initialize(input_);
			
			//safety checks
			if(!parsed) return;
			if(!MyBase){
				cout << "Input error: set " << setname << " was not found in " << input_ << "!" << endl;
				parsed = false;
				return;
			}
			if(MyBase->GetTree()==NULL) { parsed = false; return; }
			
			//initialize scanner after parsing
			scanner = new KScanner(MyBase->GetTree(),globalOpt);
		}
		//destructor
		virtual ~KScanManager() {}
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
		void Scan(){
			if(!parsed) return;
			
			//scanner does the rest
			scanner->Loop();
			
			//final stuff
			MyBase->CloseFile();
		}
		
	private:
		//member variables
		string setname;
		bool doClone;
		KScanner* scanner;
		KBase* MyBase;
};


#endif