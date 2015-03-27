#ifndef KMANAGER_H
#define KMANAGER_H

//custom headers
#include "KParser.h"
#include "KMap.h"

//ROOT headers
#include <TROOT.h>

//STL headers
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>
#include <unistd.h> //for sleep

using namespace std;

//--------------------------------------------
//base class for managers & input file parsing
class KManager {
	public:
		//constructor
		KManager() : globalOpt(0),parsed(false) {}
		KManager(string input_, string treedir_) : globalOpt(0),parsed(false) {
			//must always have an option map
			globalOpt = new OptionMap();
			//store treedir in global options
			globalOpt->Set("treedir",treedir_);
		}
		//destructor
		virtual ~KManager() {}
		//parse most initializations based on text input
		virtual void Initialize(string input_){
			parsed = Parse(input_);
			
			//final checks and initializations
			int prcsn;
			if(globalOpt->Get("yieldprecision",prcsn)) cout << fixed << setprecision(prcsn);
		}
		//parse input file
		bool Parse(string inname){
			bool parsed_ = true;
			
			//infinite loop detection
			if(inputs.find(inname)!=inputs.end()){
				cout << "Input warning: infinite loop detected! File " << inname << " has already been parsed. Check your input files." << endl;
				//averts infinite loop, but parsing can continue
				return true;
			}
			else inputs.insert(inname);
			
			string intype;
			string line;
			ifstream infile(inname.c_str());
			if(infile.is_open()){
				while(getline(infile,line)){
					//skip commented lines
					if(line[0]=='#') continue;
					//skip blank lines
					if(line.size()<2) continue;
					
					//check for carriage returns (not allowed)
					if(line[line.size()-1]=='\r') {
						cout << "Carriage return detected. Please run:" << endl;
						cout << "dos2unix " << inname << endl;
						cout << "and then try again." << endl;
						return false;
					}
					
					//check for input type
					if(line.compare(0,6,"OPTION")==0) { intype = "OPTION"; continue; }
					else if(line.compare(0,3,"SET")==0) { intype = "SET"; continue; }
					else if(line.compare(0,7,"HISTO2D")==0) { intype = "HISTO2D"; continue; }
					else if(line.compare(0,5,"HISTO")==0) { intype = "HISTO"; continue; }
					else if(line.compare(0,10,"VARIATION")==0) { intype = "VARIATION"; continue; }
					else if(line.compare(0,9,"SELECTION")==0) { intype = "SELECTION"; continue; }
					//another input file to parse on next line
					else if(line.compare(0,5,"INPUT")==0) { intype = "INPUT"; continue; }
					
					//otherwise, process line according to input type
					if(intype=="OPTION") KParser::processOption(line,globalOpt);
					else if(intype=="SET") processSet(line);
					else if(intype=="HISTO2D") processHisto(line,2);
					else if(intype=="HISTO") processHisto(line,1);
					else if(intype=="VARIATION") processVariation(line);
					else if(intype=="SELECTION") processSelection(line);
					else if(intype=="INPUT") parsed_ &= Parse(line);
				}
				parsed_ &= true;
			}
			else {
				cout << "Input error: could not open input file \"" << inname << "\"." << endl;
				parsed_ &= false;
			}
			return parsed_;
		}
		//virtual input processing functions
		virtual void processSet(string line) {}
		virtual void processHisto(string line, int dim) {}
		virtual void processVariation(string line) {}
		virtual void processSelection(string line) {}
		//accessors
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void ListOptions() {
			OMit it;
			for(it = globalOpt->GetTable().begin(); it != globalOpt->GetTable().end(); it++){
				cout << it->first /*<< ": " << it->second->value*/ << endl;
			}
		}
		
	protected:
		//member variables
		OptionMap* globalOpt;
		bool parsed;
		set<string> inputs;
};

#endif