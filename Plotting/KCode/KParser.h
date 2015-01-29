#ifndef KPARSER_H
#define KPARSER_H

//custom headers
#include "KManager.h"

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
					else if(line.compare(0,5,"HISTO")==0) { intype = "HISTO"; continue; }
					
					//otherwise, process line according to input type
					if(intype=="OPTION") processOption(line,MyManager->option);
					else if(intype=="SET") processSet(line);
					else if(intype=="HISTO") processHisto(line);
				}
				parsed = true;
			}
			else {
				cout << "Input error: could not open input file \"" << MyManager->input << "\"." << endl;
				parsed = false;
			}
			return parsed;
		}
		//helper functions
		void processOption(string line, OptionMap* option){
			//type:name[value]
			vector<string> fields;
			bool goodOption = splitOption(line,fields);
			if(!goodOption) {
				cout << "Improperly formatted option:" << endl;
				cout << line << endl;
				cout << "This option will not be added." << endl;
				return;
			}
			
			//currently anticipated option types: full/abbrev.
			//bool/b, int/i, double/d, string/s, color/c,
			//vbool/vb, vint/vi, vdouble/vd, vstring/vs, vcolor/vc (vectors)
			//others could easily be added...
			if(fields.size()>=3){
				string type = fields[0]; string name = fields[1]; string val = fields[2];
				bool isvector = type[0]=='v';
				if(isvector) type.erase(0,1);
				
				//match strings to types
				if(type=="bool" || type=="b") addOption<bool>(option,name,val,isvector);
				else if(type=="int" || type=="i") addOption<int>(option,name,val,isvector);
				else if(type=="double" || type=="d") addOption<double>(option,name,val,isvector);
				else if(type=="string" || type=="s") addOption<string>(option,name,val,isvector);
				else if(type=="color" || type=="c") addOption<Color_t>(option,name,val,isvector);
			}
		}
		//handles vector and non-vector options
		template <class O> void addOption(OptionMap* option, string name, string val, bool isvector){
			if(isvector){
				vector<O> vtmp;
				//comma-separated values
				vector<string> fields;
				process(val,',',fields);
				for(int i = 0; i < fields.size(); i++){
					vtmp.push_back(getOptionValue<O>(fields[i]));
				}
				option->Set(name,vtmp);
			}
			else {
				option->Set(name, getOptionValue<O>(val));
			}
		}
		//handles default type cases
		//special cases declared below (namespace scope, outside class declaration)
		template <class O> O getOptionValue(string val){
			stringstream sval(val);
			O tmp;
			sval >> tmp;
			return tmp;
		}
		//splits option format into three components: type:name[value]
		bool splitOption(string line, vector<string>& fields){
			//first component: type
			int colon = searchLine(line, ':');
			if(colon>-1) {
				fields.push_back(line.substr(0,colon));
				line.erase(0,colon+1);
			}
			else return false;
			
			//second component: name
			int lbracket = searchLine(line, '[');
			if(lbracket>-1){
				fields.push_back(line.substr(0,lbracket));
				line.erase(0,lbracket+1);
			}
			else return false;
			
			//third component: value
			if(line[line.size()-1]==']') line.erase(line.size()-1,1);
			fields.push_back(line);
			return true;
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
			process(line,'\t',fields);
			
			//debug: print the contents of curr_sets
			/*map<int,KBase*>::iterator dit;
			for(dit = curr_sets.begin(); dit != curr_sets.end(); dit++){
				cout << dit->second->GetName() << ", ";
			}
			cout << endl;*/
			
			//check corner cases for indent
			if(indent==0 && fields[0]=="base"){
				cout << "Input error: base must have a parent set! This input will be ignored." << endl;
				return;
			}
			else if(indent>0){
				map<int,KBase*>::iterator it = MyManager->curr_sets.find(indent-1);
				if(it == MyManager->curr_sets.end()){
					cout << "Input error: no parent can be identified for this set! There may be too many indents, or the parent was not input properly. This input will be ignored." << endl;
					return;
				}
			}
			
			//reset entries in the map of current sets that have higher (or equal) indents vs. current line
			map<int,KBase*>::iterator it = MyManager->curr_sets.lower_bound(indent);
			MyManager->curr_sets.erase(it,MyManager->curr_sets.end());
			
			if(fields.size()>=4){
				//keeps track of last used field
				unsigned ctr = 0;
			
				KBase* tmp = 0;
				if(fields[0]=="base"){
					if(fields[1]=="ext") { //external is different
						tmp = new KBaseExt(fields[2],MyManager->option);
						string filename = fields[3];
						ctr = 4;
						
						//check for specific histos to import
						if(fields.size()>ctr+1) {
							tmp->SetAddExt(true);
							TFile* f_ext = TFile::Open(filename.c_str(),"READ");
							for(unsigned i = ctr; i < fields.size(); i+=2){
								TH1F* htmp = (TH1F*)f_ext->Get(fields[i+1].c_str());
								tmp->AddHisto(fields[i],htmp);
							}
							tmp->SetAddExt(false);
						}
					}
					else{
						//intlumi for data or xsection for mc
						double property = 0;
						if(fields.size()>4) { stringstream ps(fields[4]); ps >> property; ctr = 5; }
						else ctr = 4;
						
						//create object
						string full_filename = MyManager->treedir + "/" + fields[3];
						if(fields[1]=="data") tmp = new KBaseData(fields[2],full_filename,MyManager->option,property); 
						else if(fields[1]=="mc") tmp = new KBaseMC(fields[2],full_filename,MyManager->option,property);
					}
				}
				else {
					//parse color
					Color_t color = processColor(fields[3]);
					
					//create object
					if(fields[0]=="hist" && fields[1]=="data") tmp = new KSetData(fields[2],color,MyManager->option);
					else if(fields[0]=="hist" && fields[1]=="mc") tmp = new KSetMC(fields[2],color,MyManager->option);
					else if(fields[0]=="stack" && fields[1]=="mc") tmp = new KSetMCStack(fields[2],color,MyManager->option);
					ctr = 4;
				}
				
				//store and add to parent set
				if(tmp){
					MyManager->curr_sets[indent] = tmp;
					if(indent==0) MyManager->MySets.push_back(MyManager->curr_sets[indent]); //add top-level set to manager
					else {
						MyManager->curr_sets[indent-1]->AddChild(tmp);
						//cout << curr_sets[indent-1]->GetName() << "->AddChild(" << tmp->GetName() << ")" << endl;
					}
					
					//check for additional options
					if(fields.size()>ctr) {
						for(unsigned i = ctr; i < fields.size(); i++){
							if(fields[1]=="mc"){
								if(fields[i].size()>2 && fields[i].compare(0,2,"NT")==0) MyManager->curr_sets[indent]->SetNormType(fields[i].substr(2));
								else if(fields[i].size()>3 && fields[i].compare(0,3,"NEP")==0) {
									stringstream val;
									val << fields[i].substr(3);
									int n;
									val >> n;
									MyManager->curr_sets[indent]->SetNEventProc(n);
								}
							}
							
							else if(fields[i]=="subtract") MyManager->curr_sets[indent]->SetSubtract(true);
						}
					}
				}
				else {
					cout << "Input error: set type \"" << fields[0] << " " << fields[1] << "\" is not recognized. This input will be ignored." << endl;
					return;
				}
			}
			else {
				cout << "Input error: not enough fields in this line. Check that all fields are tab-separated. This input will be ignored." << endl;
				return;
			}

		}
		Color_t processColor(string line){
			//color input is space-separated
			vector<string> fields;
			process(line,' ',fields);
			
			Color_t color = 0;
			//mapping of strings to Color_t enums (from Rtypes.h)
			if(fields[0]=="kWhite") color = kWhite;
			else if(fields[0]=="kBlack") color = kBlack;
			else if(fields[0]=="kGray") color = kGray;
			else if(fields[0]=="kRed") color = kRed;
			else if(fields[0]=="kGreen") color = kGreen;
			else if(fields[0]=="kBlue") color = kBlue;
			else if(fields[0]=="kYellow") color = kYellow;
			else if(fields[0]=="kMagenta") color = kMagenta;
			else if(fields[0]=="kCyan") color = kCyan;
			else if(fields[0]=="kOrange") color = kOrange;
			else if(fields[0]=="kSpring") color = kSpring;
			else if(fields[0]=="kTeal") color = kTeal;
			else if(fields[0]=="kAzure") color = kAzure;
			else if(fields[0]=="kViolet") color = kViolet;
			else if(fields[0]=="kPink") color = kPink;
			
			//check for addition or subtraction
			if(fields.size()==3){
				int mod;
				stringstream ms(fields[2]);
				ms >> mod;
				
				if(fields[1]=="+") color += mod;
				else if(fields[1]=="-") color -= mod;
			}
			
			return color;
		}
		void processHisto(string line){
			//cout << line << endl;
			//cout << line.size() << endl;
		
			//tab separated input
			vector<string> fields;
			process(line,'\t',fields);
			
			//first check for variable bins
			if(fields.size() > 2 && fields[0]=="bins"){
				MyManager->varbins = new double[fields.size()-1];
				for(unsigned i = 0; i < fields.size()-1; i++){
					stringstream ts(fields[i+1]);
					ts >> MyManager->varbins[i];
				}
				MyManager->nbins = fields.size()-2;
				return;
			}
			
			//keeps track of last used field
			unsigned ctr = 0;
			
			//otherwise, create a histo
			TH1F* hist = 0;
			if(fields.size() >= 4 && fields[1]=="varbin" && MyManager->varbins){ //variable binning case
				hist = new TH1F(fields[0].c_str(),"",MyManager->nbins,MyManager->varbins);
				hist->GetXaxis()->SetTitle(fields[2].c_str());
				hist->GetYaxis()->SetTitle(fields[3].c_str());
				ctr = 4;
			}
			else if(fields.size() >= 6 && fields[1]!="varbin"){ //normal binning case
				stringstream s1(fields[1]);
				s1 >> MyManager->nbins;
				double binlow, binhigh;
				stringstream s2(fields[2]), s3(fields[3]);
				s2 >> binlow; s3 >> binhigh;
				
				hist = new TH1F(fields[0].c_str(),"",MyManager->nbins,binlow,binhigh);
				hist->GetXaxis()->SetTitle(fields[4].c_str());
				hist->GetYaxis()->SetTitle(fields[5].c_str());
				ctr = 6;
			}
			
			//check for additional options
			//currently, histo options are only bools; if an opt name is present, that opt is set as enabled
			OptionMap* omap = new OptionMap();
			if(fields.size() > ctr){
				for(unsigned i = ctr; i < fields.size(); i++){
					KOption<bool>* otmp = new KOption<bool>(true);
					omap->Add(fields[i],otmp);
				}
			}
			
			KPlot* ptmp = 0;
			if(hist){
				ptmp = new KPlot(hist,omap);
				MyManager->MyPlots.Add(fields[0],ptmp);
			}
			else {
				cout << "Input error: histo input in an unrecognized format. This input will be ignored." << endl;
				return;
			}
			
		}
		//generalization for processing a line
		void process(string line, char delim, vector<string>& fields){
			stringstream ss(line);
			string field;
			while(getline(ss,field,delim)){
				fields.push_back(field);
			}
		}
		//finds the first location of a given char in a string
		int searchLine(string line, char val){
			for(int i = 0; i < line.size(); i++){
				if(line[i]==val) return i;
			}
			return -1;
		}
	
	private:
		//member variables
		KManager* MyManager;
		bool parsed;

};

//function template specializations
//special case: just take the string directly
template <> string KParser::getOptionValue<string>(string val){
	return val;
}
//special case: special processing for colors
template <> Color_t KParser::getOptionValue<Color_t>(string val){
	return processColor(val);
}

//implementation of KManager constructor
//note: dir="tree" default
KManager::KManager(string in, string dir) : input(in),treedir(dir),option(0),numer(0),denom(0),yieldref(0),doPrint(false),varbins(0),nbins(0),parsed(false) {
	//parser does most initializations based on text input
	option = new OptionMap();
	MyParser = new KParser(this);
	parsed = MyParser->Parse();
	
	//final checks and initializations
	MyRatio = new KSetRatio(option);
	if(option->Get("calcfaketau",false)) FakeTauEstimationInit();
	//store correction root files centrally
	//todo: make file location and histo name configurable
	if(option->Get("pucorr",true)) {
		TFile* pufile = new TFile("corrections/puWeightsLQ.root","READ"); //puWeights
		TH1F* puWeights = (TH1F*)pufile->Get("pileup");
		option->Set("puWeights",puWeights);
	}
	if(option->Get("mucorr",true)) {
		TFile* mufile = new TFile("corrections/muIDTight.root","READ"); //puWeights
		TH1F* muIDTight = (TH1F*)mufile->Get("muIDTight");
		option->Set("muIDTight",muIDTight);
	}
}

#endif