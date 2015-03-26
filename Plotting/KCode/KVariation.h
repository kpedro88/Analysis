#ifndef KVARIATION_H
#define KVARIATION_H

//custom headers
#include "KMap.h"

//ROOT headers

//STL headers
#include <string>
#include <vector>

using namespace std;

//forward declaration
class KSkimmer;

//----------------------------------------------------------------
//base class for Variators, has standard functions defined
class KVariator {
	public:
		//constructor
		KVariator() : name(""), localOpt(0), skimmer(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
		}
		KVariator(string name_, OptionMap* localOpt_) : name(name_), localOpt(localOpt_), skimmer(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
		}
		//accessors
		string GetName() { return name; }
		void SetSkimmer(KSkimmer* skimmer_) { skimmer = skimmer_; }
		//functions
		virtual void DoVariation() {}
		virtual void UndoVariation() {}
		
	protected:
		//member variables
		string name;
		OptionMap* localOpt;
		KSkimmer* skimmer;
};

//----------------------------------------------------------------
//class to keep track of a list of Variators
class KVariation {
	public:
		//constructor
		KVariation() : name(""), skimmer(0) {}
		KVariation(string name_) : name(name_), skimmer(0) {}
		//destructor
		virtual ~KVariation() {}
		//accessors
		string GetName() { return name; }
		void AddVariator(KVariator* var_){
			variatorList.push_back(var_);
			for(unsigned s = 0; s < variatorList.size(); s++){
				variatorList[s]->SetSkimmer(skimmer_);
			}
		}
		void SetSkimmer(KSkimmer* skimmer_){ skimmer = skimmer_; }
		void DoVariation() {
			for(unsigned v = 0; v < variatorList.size(); v++){
				variatorList[v]->DoVariation();
			}
		}
		void UndoVariation() {
			for(unsigned v = 0; v < variatorList.size(); v++){
				variatorList[v]->UndoVariation();
			}
		}
		
	protected:
		//member variables
		string name;
		KSkimmer* skimmer;
		vector<KVariator*> variatorList;
};

#endif