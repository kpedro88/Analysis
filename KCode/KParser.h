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
#include <set>
#include <array>

//forward declarations
class KSelector;
class KVariator;

template <size_t N>
class KNamedN {
	public:
		//constructor defined below, uses KParser functions
		KNamedN(vector<string> fields_) {
			assert(fields_.size()>=N);
			for(size_t i = 0; i < N; ++i){
				fields[i] = fields_[i];
			}
			//everything after the starting fields is an option
			optfields.reserve(fields_.size()-N);
			optfields.insert(optfields.end(),fields_.begin()+N,fields_.end());
		}
		
		//generate option map on the fly (to avoid sharing)
		//defined below, uses KParser
		OptionMap* localOpt();
		
		//members
		array<string,N> fields;
		vector<string> optfields;
		vector<KNamedN<N>*> children;
};
//typedefs
typedef KNamedN<1> KNamed;
typedef KNamedN<3> KNamedBase;

class KParserOption {
	public:
		//constructor
		//formats:
		//1. type:name[value]
		//2. vtype:name[value]
		//3. vtype{sep}:name[value]
		//4. type+:name[value]
		//5. vtype+:name[value]
		//6. vtype{sep}+:[value]
		KParserOption(string line) : sep(','), isvector(false), valid(true), append(false) {
			//first component: type
			string::size_type colon = line.find(':');
			if(colon!=string::npos){
				type = line.substr(0,colon);
				line.erase(0,colon+1);
			}
			else valid |= false;
			
			//check for vector formats
			isvector = type[0]=='v';
			if(isvector) type.erase(0,1);
			
			//check for separator
			string::size_type curly1 = type.find('{');
			string::size_type curly2 = type.find('}');
			if(curly2!=string::npos and curly2<=type.size()+1 and curly2-curly1==2){
				sep = type[curly1+1];
				type.erase(curly1,3);
			}
			
			//check for appending
			string::size_type plus = type.find('+');
			if(plus!=string::npos){
				append = true;
				type.erase(plus,1);
			}

			//remove whitespace from front
			while(type[0]==' '){
				type.erase(0,1);
			}
			
			//second component: name
			string::size_type lbracket = line.find('[');
			if(lbracket!=string::npos){
				name = line.substr(0,lbracket);
				line.erase(0,lbracket+1);
			}
			else valid |= false;
			
			//third component: value
			if(line[line.size()-1]==']') line.erase(line.size()-1,1);
			value = line;
		}
		
		//members
		string type, name, value;
		char sep;
		bool isvector, valid, append;
};

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
	//forward declaration
	void processOption(string line, OptionMap* option);
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
	template <class O> void addOption(OptionMap* option, const KParserOption& opt){
		if(opt.isvector){
			vector<O> vtmp;
			if(opt.append) option->Get(opt.name,vtmp);
			//comma-separated values
			vector<string> fields;
			process(opt.value,opt.sep,fields);
			for(unsigned i = 0; i < fields.size(); i++){
				vtmp.push_back(getOptionValue<O>(fields[i]));
			}
			option->Set(opt.name,vtmp);
		}
		else {
			O tmp;
			if(opt.append) {
				option->Get(opt.name,tmp);
				tmp += getOptionValue<O>(opt.value);
			}
			else tmp = getOptionValue<O>(opt.value);
			option->Set(opt.name, tmp);
		}
	}
	//special handling for chains
	template <> void addOption<KChain>(OptionMap* option, const KParserOption& opt){
		//comma-separated values
		vector<string> fields;
		process(opt.value,opt.sep,fields);
		
		if(opt.isvector){
			vector<KChain> vtmp;
			if(opt.append) option->Get(opt.name,vtmp);
			for(unsigned i = 0; i < fields.size()-3; i+=4){
				vtmp.emplace_back(vector<string>(fields.begin()+i,fields.begin()+min(i+4,unsigned(fields.size()))));
			}
			option->Set(opt.name,vtmp);
		}
		else {
			KChain ch(fields);
			option->Set(opt.name,ch);
		}
	}
	//special handling for input files
	void getOptionValueInput(OptionMap* option, string val){
		//infinite loop detection
		set<string> inputs; option->Get("inputs",inputs);
		if(inputs.find(val)!=inputs.end()){
			cout << "Input warning: infinite loop detected! File " << val << " has already been parsed. Check your input files." << endl;
			//averts infinite loop, but parsing can continue
			return;
		}
		else {
			inputs.insert(val);
			//reset input list
			option->Set("inputs",inputs);
		}
		ifstream instream(val.c_str());
		string line;
		if(instream.is_open()){
			while(getline(instream,line)){
				//skip commented lines
				if(line[0]=='#') continue;
				//skip blank lines
				if(line.size()<2) continue;
				
				//check for carriage returns (not allowed)
				if(line.back()=='\r') {
					line.pop_back();
				}
				
				//process line
				KParser::processOption(line,option);
			}
		}
	}
	void addOptionInput(OptionMap* option, const KParserOption& opt){
		//name is not used
		if(opt.isvector){
			//comma-separated values
			vector<string> fields;
			process(opt.value,opt.sep,fields);
			for(unsigned i = 0; i < fields.size(); i++){
				getOptionValueInput(option,fields[i]);
			}
		}
		else {
			getOptionValueInput(option,opt.value);
		}
	}
	//helper functions
	void processOption(string line, OptionMap* option){
		//(v)type({sep})(+):name[value]
		KParserOption opt(line);
		if(!opt.valid) {
			cout << "Improperly formatted option:" << endl;
			cout << line << endl;
			cout << "This option will not be added." << endl;
			return;
		}
		
		//currently anticipated option types: full/abbrev.
		//bool/b, int/i, uint/u, float/f, double/d, string/s, color/c, chain/ch, input/in
		//vbool/vb, vint/vi, vuint/vu, vfloat/vf, vdouble/vd, vstring/vs, vcolor/vc, vchain/vch, vinput/vin (vectors)
		//others could easily be added...

		//can only append to vectors or strings
		if(opt.append and !opt.isvector and opt.type!="string" and opt.type!="s"){
			cout << "Cannot append to option type " << opt.type << "; skipping " << opt.name << endl;
			return;
		}
		
		//match strings to types
		if(opt.type=="bool" || opt.type=="b") addOption<bool>(option,opt);
		else if(opt.type=="int" || opt.type=="i") addOption<int>(option,opt);
		else if(opt.type=="uint" || opt.type=="u") addOption<unsigned>(option,opt);
		else if(opt.type=="float" || opt.type=="f") addOption<float>(option,opt);
		else if(opt.type=="double" || opt.type=="d") addOption<double>(option,opt);
		else if(opt.type=="string" || opt.type=="s") addOption<string>(option,opt);
		else if(opt.type=="color" || opt.type=="c") addOption<Color_t>(option,opt);
		else if(opt.type=="chain" || opt.type=="ch") addOption<KChain>(option,opt);
		else if(opt.type=="input" || opt.type=="in") addOptionInput(option,opt);
		else {
			cout << "Unknown option type: " << line << endl;
		}
	}
	template <size_t N>
	KNamedN<N>* processNamed(string line){
		//tab separated input
		vector<string> fields;
		process(line,'\t',fields);
		KNamedN<N>* tmp = new KNamedN<N>(fields);
		return tmp;
	}
	KSelector* processSelector(KNamed* tmp);
	KVariator* processVariator(KNamed* tmp);
}

template<size_t N>
OptionMap* KNamedN<N>::localOpt() {
	OptionMap* omap = new OptionMap();
	for(auto& field : optfields){
		KParser::processOption(field,omap);
	}
	return omap;
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
// processBase() defined in KBase.h, processSelector() defined in KSelection.h, processVariator() defined in KVariation.h

#endif
