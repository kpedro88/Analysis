#ifndef KMAP_H
#define KMAP_H

//ROOT headers
#include <TROOT.h>
#include <TH1.h>
#include <THStack.h>
#include <TGraphAsymmErrors.h>

//STL headers
#include <string>
#include <map>
#include <vector>
#include <utility>

using namespace std;

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
			typename map<string,T>::iterator lb = table.lower_bound(name);
			
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
		map<string,T>& GetTable() { return table; }
		//accessor
		T Get(string name) {
			if(stmp==name) return itmp->second;
			else{
				typename map<string,T>::iterator it = table.find(name);
				if(it == table.end()) return T(); //default object
				else return it->second;
			}
		}
		//check if entry exists
		bool Has(string name) {
			typename map<string,T>::iterator it = table.find(name);
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
			typename map<string,T*>::iterator lb = table.lower_bound(name);
			
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
			typename map<string,T*>::iterator it = table.find(name);
			if(it == table.end()) return NULL;
			else return it->second;
		}
		
	protected:	
		//member variable
		map<string,T*> table;
};

//-------------------------------------------------
//structs for option types (e.g. bool, int, double)
struct KOpt {
	
};
template <class O>
struct KOption : public KOpt {
	KOption(O val) : value(val) {}
	O value;
};

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
			typename map<string,KOpt*>::iterator it = table.find(name);
			return it != table.end();
		}
};



class KPlot;
class KSelector;
class KSelection;
class KVariation;
typedef pair<string, OptionMap*> KNamed;
typedef map<string,KOpt*>::iterator OMit;
typedef map<string,TH1*>::iterator HMit;
typedef map<string,THStack*>::iterator SMit;
typedef map<string,TGraphAsymmErrors*>::iterator EMit;
typedef map<string,KPlot*>::iterator PMit;
typedef map<string,OptionMap*>::iterator OMMit;
typedef map<string,KSelector*>::iterator SrMit;
typedef map<string,KSelection*>::iterator SnMit;
typedef KMap<TH1*> HistoMap;
typedef KMap<THStack*> StackMap;
typedef KMap<TGraphAsymmErrors*> ErrorMap;
typedef KMap<KPlot*> PlotMap;
typedef KMap<OptionMap*> OptionMapMap;
typedef KMap<PlotMap*> PlotMapMap;
typedef map<string,PlotMap*>::iterator PMMit;
typedef KMap<string,KSelector*> SelectorMap;
typedef KMap<string,KSelection*> SelectionMap;
typedef KMap<string,KVariation*> VariationMap;

#endif