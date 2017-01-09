#ifndef KMANAGER_H
#define KMANAGER_H

//custom headers
#include "KParser.h"
#include "KMap.h"
#include "KVariation.h"
#include "KSelection.h"

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
#include <algorithm>
#include <iterator>

using namespace std;

//--------------------------------------------
//base class for managers & input file parsing
class KManager {
	public:
		//constructor
		KManager() : globalOpt(0),parsed(false) {}
		KManager(string treedir_) : globalOpt(0),parsed(false) {
			//must always have an option map
			globalOpt = new OptionMap();
			//store treedir in global options
			globalOpt->Set("treedir",treedir_);
			//initialize iterators
			curr_sel = allSelections.GetTable().end();
			curr_var = allVariations.GetTable().end();
		}
		//destructor
		virtual ~KManager() {
			if(globalOpt->Get("listoptions",false)) ListOptions();			
		}
		//parse most initializations based on text input
		virtual void Initialize(vector<string>& infiles_, vector<string>& direct_){
			parsed = true;
			for(auto& input_ : infiles_){
				parsed &= ParseFile(input_);
			}
			if(direct_.size()>0) parsed &= ParseDirect(direct_);
			
			//final checks and initializations
			int prcsn;
			if(globalOpt->Get("yieldprecision",prcsn)) cout << fixed << setprecision(prcsn);
		}
		//parse input file
		bool ParseFile(string inname){
			bool parsed_ = true;
			
			//infinite loop detection
			if(inputs.find(inname)!=inputs.end()){
				cout << "Input warning: infinite loop detected! File " << inname << " has already been parsed. Check your input files." << endl;
				//averts infinite loop, but parsing can continue
				return true;
			}
			else inputs.insert(inname);
			
			ifstream infile(inname.c_str());
			if(infile.is_open()){
				parsed_ &= ParseStream(infile);
			}
			else {
				cout << "Input error: could not open input file \"" << inname << "\"." << endl;
				parsed_ &= false;
			}
			return parsed_;
		}
		bool ParseDirect(vector<string>& input){
			stringstream sinput;
			copy(input.begin(), input.end(), ostream_iterator<string>(sinput, "\n")); sinput << "\n";
			return ParseStream(sinput);
		}
		bool ParseStream(istream& instream){
			bool parsed_ = true;
			string intype;
			string line;
			while(getline(instream,line)){
				//skip commented lines
				if(line[0]=='#') continue;
				//skip blank lines
				if(line.size()<2) continue;
				
				//check for carriage returns (not allowed)
				if(line.back()=='\r') {
					line.pop_back();
				}
				
				//check for input type
				if(line.compare(0,6,"OPTION")==0) { intype = "OPTION"; continue; }
				else if(line.compare(0,3,"SET")==0) { intype = "SET"; continue; }
				else if(line.compare(0,7,"HISTO2D")==0) { intype = "HISTO2D"; continue; }
				else if(line.compare(0,5,"HISTO")==0) { intype = "HISTO"; continue; }
				else if(line.compare(0,10,"VARIATION")==0) { intype = "VARIATION"; continue; }
				else if(line.compare(0,9,"SELECTION")==0) { intype = "SELECTION"; continue; }
				else if(line.compare(0,5,"STYLE")==0) { intype = "STYLE"; continue; }
				//another input file to parse on next line
				else if(line.compare(0,5,"INPUT")==0) { intype = "INPUT"; continue; }
				
				//otherwise, process line according to input type
				if(intype=="OPTION") KParser::processOption(line,globalOpt);
				else if(intype=="SET") processSet(line);
				else if(intype=="HISTO2D") processHisto(line,2);
				else if(intype=="HISTO") processHisto(line,1);
				else if(intype=="VARIATION") processVariation(line);
				else if(intype=="SELECTION") processSelection(line);
				else if(intype=="STYLE") processStyle(line);
				else if(intype=="INPUT") parsed_ &= ParseFile(line);
			}
			return parsed_;
		}
		//virtual input processing functions
		virtual void processSet(string) {}
		virtual void processHisto(string, int) {}
		//wait to construct variations until we know which are desired and with which selections to use them
		//for now, just store the lines for each defined variation in a vector, and construct variators with them later
		virtual void processVariation(string line){
			if(line[0]=='\t'){ //variator
				line.erase(0,1);
				KNamed* tmp = KParser::processNamed<1>(line);
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
		virtual void processSelection(string line){
			if(line[0]=='\t'){ //selector
				line.erase(0,1);
				KNamed* tmp = KParser::processNamed<1>(line);
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
		//generalized function to make selection
		KSelection* makeSelection(string selection){
			//check for systematic
			vector<string> sel_unc;
			KParser::process(selection,'_',sel_unc);
			string sel = sel_unc[0];
			string unc = "";
			if(sel_unc.size()>1) unc = sel_unc[1];
			
			//check in full list (selections may be repeated with different systematics, so never remove things from the full list)
			if(allSelections.Has(sel)){
				KVariation* vntmp = 0;
				if(unc.size()>0) {
					if(!allVariations.Has(unc)) {
						cout << "Input error: variation " << unc << " is not defined. This request will be ignored." << endl;
						return NULL;
					}
					vntmp = new KVariation(unc);
					
					//create variators for variation
					vector<KNamed*> variatorLines = allVariations.Get(unc);
					for(unsigned v = 0; v < variatorLines.size(); v++){
						KVariator* vrtmp = KParser::processVariator(variatorLines[v]);
						if(vrtmp) vntmp->AddVariator(vrtmp);
					}
				}
			
				//create selection using full name (sel + unc)
				KSelection* sntmp = new KSelection(selection,globalOpt);
				if(vntmp) sntmp->SetVariation(vntmp);
				
				//create selectors for selection
				vector<KNamed*> selectorLines = allSelections.Get(sel);
				for(unsigned s = 0; s < selectorLines.size(); s++){
					KSelector* srtmp = KParser::processSelector(selectorLines[s]);
					if(srtmp) sntmp->AddSelector(srtmp);
				}
				
				return sntmp;
			}
			else {
				cout << "Input error: selection " << sel << " is not defined. This request will be ignored." << endl;
				return NULL;
			}
		}
		//wait to construct styles until one is requested
		//just keep the option list as a string (make a new instance for each request)
		virtual void processStyle(string style) {
			size_t firsttab = style.find('\t');
			string name = "";
			string opts = "";
			if(firsttab != string::npos){
				name = style.substr(0,firsttab);
				opts = style.substr(firsttab+1,string::npos);
			}
			else name = style; //style with no options
			
			allStyles.Add(name,opts);
		}
		//accessors
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void ListOptions() {
			cout << globalOpt->GetTable() << endl;
		}
		
	protected:
		//member variables
		OptionMap* globalOpt;
		bool parsed;
		set<string> inputs;
		KMap<string> allStyles;
		KMap<vector<KNamed*> > allSelections, allVariations;
		map<string,vector<KNamed*> >::iterator curr_sel, curr_var;
};

#endif
