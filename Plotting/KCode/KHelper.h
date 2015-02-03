#ifndef KHELPER_H
#define KHELPER_H

//custom headers
#include "KMap.h"

//ROOT headers
#include <TROOT.h>
#include <TMath.h>

//STL headers
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace std;

namespace KMath {
	//helper functions
	double phi(double x, double y) {
		double phi_ = atan2(y, x);
		return (phi_>=0) ?  phi_ : phi_ + 2*TMath::Pi();
	}
	double DeltaPhi(double phi1, double phi2) {
		double phi1_= phi( cos(phi1), sin(phi1) );
		double phi2_= phi( cos(phi2), sin(phi2) );
		double dphi_= phi1_-phi2_;
		if( dphi_> TMath::Pi() ) dphi_-=2*TMath::Pi();
		if( dphi_<-TMath::Pi() ) dphi_+=2*TMath::Pi();

		return dphi_;
	}
	double DeltaR(double phi1, double eta1, double phi2, double eta2){
		double dphi = DeltaPhi(phi1,phi2);
		double deta = eta2 - eta1;
		double dR2 = dphi*dphi + deta*deta;
		return sqrt(dR2);
	}
}

namespace KText {
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
			for(int i = 0; i < fields.size(); i++){
				vtmp.push_back(getOptionValue<O>(fields[i]));
			}
			option->Set(name,vtmp);
		}
		else {
			option->Set(name, getOptionValue<O>(val));
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

}


#endif