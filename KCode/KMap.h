#ifndef KMAP_H
#define KMAP_H

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <THStack.h>
#include <TGraphAsymmErrors.h>

//STL headers
#include <string>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <exception>

using namespace std;


//------------------------------------------------------
//general helpers (placed here arbitrarily)
TFile* KOpen(string fname, string mode="READ", bool dothrow=true){
	TFile* file = TFile::Open(fname.c_str(), mode.c_str());
	if(!file and dothrow){
		throw runtime_error("Could not open file "+fname);
	}
	return file;
}
template <class T, class F=TFile>
T* KGet(F* file, string getname, bool dothrow=true){
	T* obj = static_cast<T*>(file->Get(getname.c_str()));
	if(!obj and dothrow){
		throw runtime_error("Could not get "+getname+" from file "+file->GetName());
	}
	return obj;
}

//------------------------------------------------------
//special map class with key=string and optimized insert
template <class T>
class KMap {
	public:
		//constructor
		KMap() : stmp(""), itmp(table.end()) {}
		//destructor
		virtual ~KMap() {}
		//histo map manipulators
		typename map<string,T>::iterator Add(string name, T obj){
			//finding lower bound is faster for inserting
			//lower bound: first element in map with key >= name
			auto lb = (name==stmp) ? itmp : table.lower_bound(name);
			
			//key_comp: false if name >= key, true if name < key
			if(lb != table.end() && !(table.key_comp()(name,lb->first))){
				//overwrite existing object
				lb->second = obj;
			}
			else{
				//no existing object, add it
				lb = table.insert(lb,make_pair(name,obj));
			}
			
			return lb;
		}
		map<string,T>& GetTable() { return table; }
		//accessor
		T Get(string name) {
			if(stmp==name) return itmp->second;
			else{
				auto it = table.find(name);
				if(it == table.end()) return T(); //default object
				else return it->second;
			}
		}
		//check if entry exists
		bool Has(string name) {
			auto it = table.find(name);
			if(it == table.end()) return false; //entry not present
			else {
				//store in tmps in case of subsequent Get()
				stmp = name;
				itmp = it;
				return true;
			}
		}
		
	protected:	
		//member variable
		map<string,T> table;
		string stmp;
		typename map<string,T>::iterator itmp;
};

//-----------------------------------------------------
//specialization for value=pointer
//Get() functions as Has() (false = NULL, true = !NULL)
template <class T>
class KMap<T*> {
	public:
		//destructor
		virtual ~KMap() {}
		//histo map manipulators
		typename map<string,T*>::iterator Add(string name, T* obj){
			//finding lower bound is faster for inserting
			//lower bound: first element in map with key >= name
			auto lb = table.lower_bound(name);
			
			//key_comp: false if name >= key, true if name < key
			if(lb != table.end() && !(table.key_comp()(name,lb->first))){
				//overwrite existing object
				delete lb->second;
				lb->second = obj;
			}
			else{
				//no existing object, add it
				lb = table.insert(lb,make_pair(name,obj));
			}
			
			return lb;
		}
		map<string,T*>& GetTable() { return table; }
		//accessor
		T* Get(string name) {
			auto it = table.find(name);
			if(it == table.end()) return NULL;
			else return it->second;
		}
		
	protected:	
		//member variable
		map<string,T*> table;
};

//-------------------------------------------------
//overloaded operator<< for printing STL vectors
template <typename T>
std::ostream& operator<< (std::ostream& out, const vector<T>& v) {
	if ( !v.empty() ) {
		for(unsigned iv = 0; iv < v.size(); ++iv){
			cout << v[iv];
			if(iv < v.size()-1) cout << ",";
		}
	}
	else {
		out << "[empty vector]";
	}
	return out;
}
//overloaded operator<< for printing STL pairs
template <typename T1, typename T2>
std::ostream& operator<< (std::ostream& out, const pair<T1,T2>& p) {
	out << p.first << ": " << p.second;
	return out;
}
//overloaded operator<< for printing STL maps
template <typename T1, typename T2>
std::ostream& operator<< (std::ostream& out, const map<T1,T2>& m) {
	if ( !m.empty() ) {
		for(auto& mit : m){
			cout << mit << endl;
		}
	}
	else {
		out << "[empty map]";
	}
	return out;
}
//overloaded operator<< for printing STL sets
template <typename T>
std::ostream& operator<< (std::ostream& out, const set<T>& s) {
	if ( !s.empty() ) {
		for(auto& sit : s){
			cout << sit << endl;
		}
	}
	else {
		out << "[empty set]";
	}
	return out;
}

//-------------------------------------------------
//structs for option types (e.g. bool, int, double)
struct KOpt {
	virtual ~KOpt() {}
	virtual void Print() const {}
};
template <class O>
struct KOption : public KOpt {
	KOption(O val) : value(val) {}
	virtual ~KOption() {}
	virtual void Print() const { cout << value; }
	O value;
};
//overloaded operator<< for printing KOpt
std::ostream& operator<< (std::ostream& out, const KOpt* k) {
  k->Print();
  return out;
}

//--------------------------------------
//map for options with special accessors
class OptionMap : public KMap<KOpt*> {
	public:
		//puts option value into ret
		//with a return status that indicates if the option was actually found
		template <class O> bool Get(string name, O &ret){
			KOption<O>* otmp = static_cast<KOption<O>*>(KMap<KOpt*>::Get(name));
			if(otmp) {
				ret = otmp->value;
				return true;
			}
			else return false;
		}
		
		//specialization for bools - just returns option value
		//technically an overload because function specializations are a pain in the ass
		//with ret used as default setting (enabled or disabled) when option is not found
		bool Get(string name, bool ret){
			KOption<bool>* otmp = static_cast<KOption<bool>*>(KMap<KOpt*>::Get(name));
			if(otmp) return otmp->value;
			else return ret;
		}
		
		//streamlines the process of adding or changing options
		template <class O> void Set(string name, O val){
			KOpt* otmp = new KOption<O>(val);
			Add(name,otmp);
		}
		
		//check if entry exists
		bool Has(string name) {
			auto it = table.find(name);
			return it != table.end();
		}
};



class KPlot;
class KHisto;
typedef KMap<TH1*> HistoMap;
typedef KMap<KHisto*> KHistoMap;
typedef KMap<THStack*> StackMap;
typedef KMap<TGraphAsymmErrors*> ErrorMap;
typedef KMap<KPlot*> PlotMap;
typedef KMap<OptionMap*> OptionMapMap;
typedef KMap<PlotMap*> PlotMapMap;
typedef KMap<HistoMap*> HistoMapMap;

#endif
