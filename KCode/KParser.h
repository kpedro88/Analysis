#ifndef KPARSER_H
#define KPARSER_H

//custom headers
#include "KMap.h"

//ROOT headers
#include <TROOT.h>

//STL headers
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <cmath>

//forward declarations
class KSelector;
class KVariator;

class KChain {
	public:
		//general constructors
		KChain() {}
		KChain(vector<string> fields_) {
			//chains can be lumped together
			for(unsigned i = 0; i < fields_.size()-3; i+=4){
				*this += MakeChain(vector<string>(fields_.begin()+i,fields_.begin()+min(i+4,unsigned(fields_.size()))));
			}
		}
		//standalone constructor
		static KChain MakeChain(vector<string> fields_);
		//operators
		KChain& operator+=(const KChain& ch2){
			fields.insert(fields.end(),ch2.fields.begin(),ch2.fields.end());
			if(name.size()>0) name += "_" + ch2.name;
			else name = ch2.name;
			files.insert(files.end(),ch2.files.begin(),ch2.files.end());
			return *this;
		}
		KChain operator+(const KChain& ch2){
			KChain ch1(*this);
			ch1.fields.insert(ch1.fields.end(),ch2.fields.begin(),ch2.fields.end());
			if(name.size()>0) ch1.name += "_" + ch2.name;
			else ch1.name = ch2.name;
			ch1.files.insert(ch1.files.end(),ch2.files.begin(),ch2.files.end());
			return ch1;
		}
		//accessors
		const vector<string>& GetFields() const { return fields; }
		const string& GetName() const { return name; }
		const vector<string>& GetFiles() const { return files; }
		
	private:
		//member variables
		vector<string> fields;
		string name;
		vector<string> files;
};

std::ostream& operator<< (std::ostream& out, const KChain& ch) {
	out << ch.GetName();
	return out;
}

namespace KParser {
	//generalization for processing a line
	void process(string line, char delim, vector<string>& fields){
		stringstream ss(line);
		string field;
		while(getline(ss,field,delim)){
			fields.push_back(field);
		}
	}
	//clean out forbidden characters (for filenames)
	//ref: http://codereview.stackexchange.com/questions/283/function-for-removing-forbidden-characters
	void clean(string& line, char alt='_'){
		string illegalChars = "\\/:?\"<>|";
		string::iterator it;
		for (it = line.begin(); it != line.end(); ++it){
			bool found = illegalChars.find(*it) != string::npos;
			if(found){
				*it = alt;
			}
		}
	}
	//turns text input into ROOT colors: kXXX +/- #
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
	//splits option format into three components: type:name[value]
	bool splitOption(string line, vector<string>& fields){
		//first component: type
		string::size_type colon = line.find(':');
		if(colon!=string::npos) {
			fields.push_back(line.substr(0,colon));
			line.erase(0,colon+1);
		}
		else return false;
		
		//second component: name
		string::size_type lbracket = line.find('[');
		if(lbracket!=string::npos){
			fields.push_back(line.substr(0,lbracket));
			line.erase(0,lbracket+1);
		}
		else return false;
		
		//third component: value
		if(line[line.size()-1]==']') line.erase(line.size()-1,1);
		fields.push_back(line);
		return true;
	}
	//handles default type cases
	//special cases declared below
	template <class O> O getOptionValue(string val){
		stringstream sval(val);
		O tmp;
		sval >> tmp;
		return tmp;
	}
	//function template specializations
	//special case: just take the string directly
	template <> string getOptionValue<string>(string val){
		return val;
	}
	//special case: special processing for colors
	template <> Color_t getOptionValue<Color_t>(string val){
		return processColor(val);
	}
	//handles vector and non-vector options
	template <class O> void addOption(OptionMap* option, string name, string val, bool isvector){
		if(isvector){
			vector<O> vtmp;
			//comma-separated values
			vector<string> fields;
			process(val,',',fields);
			for(unsigned i = 0; i < fields.size(); i++){
				vtmp.push_back(getOptionValue<O>(fields[i]));
			}
			option->Set(name,vtmp);
		}
		else {
			option->Set(name, getOptionValue<O>(val));
		}
	}
	//special handling for chains
	template <> void addOption<KChain>(OptionMap* option, string name, string val, bool isvector){
		//comma-separated values
		vector<string> fields;
		process(val,',',fields);
		
		if(isvector){
			vector<KChain> vtmp;
			for(unsigned i = 0; i < fields.size()-3; i+=4){
				vtmp.emplace_back(vector<string>(fields.begin()+i,fields.begin()+min(i+4,unsigned(fields.size()))));
			}
			option->Set(name,vtmp);
		}
		else {
			KChain ch(fields);
			option->Set(name,ch);
		}
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
		//bool/b, int/i, uint/u, float/f, double/d, string/s, color/c, chain/ch
		//vbool/vb, vint/vi, vuint/vu, vfloat/vf, vdouble/vd, vstring/vs, vcolor/vc, vchain/vch(vectors)
		//others could easily be added...
		if(fields.size()>=3){
			string type = fields[0]; string name = fields[1]; string val = fields[2];
			
			//remove whitespace from front
			while(type[0]==' '){
				type.erase(0,1);
			}
			
			bool isvector = type[0]=='v';
			if(isvector) type.erase(0,1);
			
			//match strings to types
			if(type=="bool" || type=="b") addOption<bool>(option,name,val,isvector);
			else if(type=="int" || type=="i") addOption<int>(option,name,val,isvector);
			else if(type=="uint" || type=="u") addOption<unsigned>(option,name,val,isvector);
			else if(type=="float" || type=="f") addOption<float>(option,name,val,isvector);
			else if(type=="double" || type=="d") addOption<double>(option,name,val,isvector);
			else if(type=="string" || type=="s") addOption<string>(option,name,val,isvector);
			else if(type=="color" || type=="c") addOption<Color_t>(option,name,val,isvector);
			else if(type=="chain" || type=="ch") addOption<KChain>(option,name,val,isvector);
			else {
				cout << "Unknown option type: " << line << endl;
			}
		}
	}
	KNamed* processNamed(string line){
		//tab separated input
		vector<string> fields;
		process(line,'\t',fields);
		string name = fields[0];
		OptionMap* omap = new OptionMap();
		
		//everything after the name is an option
		for(unsigned i = 1; i < fields.size(); i++){
			processOption(fields[i],omap);
		}
		
		//return local options for later use
		KNamed* tmp = new pair<string,OptionMap*>(name,omap);
		return tmp;
	}
	KSelector* processSelector(KNamed* tmp);
	KVariator* processVariator(KNamed* tmp);
}

KChain KChain::MakeChain(vector<string> fields_){
	KChain ch;
	ch.fields = fields_;
	//concatenate to get short name
	for(auto& field : ch.fields) ch.name += field;
	
	if(ch.fields.size()!=4) {
		cout << "Input error: chain type needs 4 fields (filepre, filemin, filemax, filesuff), given " << ch.fields.size() << ". The chain " << ch.name << " will not be initialized." << endl;
	}
	
	//chain has: filepre, filemin, filemax, filesuff
	string filepre = KParser::getOptionValue<string>(ch.fields[0]);
	int filemin = KParser::getOptionValue<int>(ch.fields[1]);
	int filemax = KParser::getOptionValue<int>(ch.fields[2]);
	string filesuff = KParser::getOptionValue<string>(ch.fields[3]);
	
	if(filemin>filemax){
		cout << "Input error: chain specified incorrectly with filemin > filemax. The chain " << ch.name << " will not be initialized." << endl;
	}
	
	//loop over ranges (inclusive)
	//todo: add option to exclude certain files?
	ch.files.reserve(filemax-filemin+1);
	for(int f = filemin; f <= filemax; f++){
		stringstream fs;
		fs << filepre << f << filesuff;
		ch.files.push_back(fs.str());
	}
	
	return ch;
}

//NOTE: namespace also includes:
// processBase() defined in KSet.h, processSelector() defined in K*Selectors.h, processVariator() defined in K*Variators.h

#endif
