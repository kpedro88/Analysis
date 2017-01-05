#ifndef KVARIATION_H
#define KVARIATION_H

//custom headers
#include "KLooper.h"
#include "KBase.h"
#include "KMap.h"

//ROOT headers

//STL headers
#include <string>
#include <vector>

using namespace std;

//----------------------------------------------------------------
//base class for Variators, has standard functions defined
class KVariator {
	public:
		//constructor
		KVariator() : name(""), localOpt(0), looper(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
		}
		KVariator(string name_, OptionMap* localOpt_) : name(name_), localOpt(localOpt_), looper(0) {
			//must always have local option map
			if(localOpt==0) localOpt = new OptionMap();
		}
		//accessors
		string GetName() { return name; }
		void SetBase(KBase* base_) { base = base_; }
		//functions
		virtual void CheckBranches() {}
		virtual void DoVariation() {}
		virtual void UndoVariation() {}
		
	protected:
		//member variables
		string name;
		OptionMap* localOpt;
		KLooper* looper;
		KBase* base;
};

//----------------------------------------------------------------
//class to keep track of a list of Variators
class KVariation {
	public:
		//constructor
		KVariation() : name(""), looper(0) {}
		KVariation(string name_) : name(name_), looper(0) {}
		//destructor
		virtual ~KVariation() {}
		//accessors
		string GetName() { return name; }
		void AddVariator(KVariator* var_){
			variatorList.push_back(var_);
		}
		void SetBase(KBase* base_){
			base = base_;
			looper = base->GetLooper();
			for(unsigned v = 0; v < variatorList.size(); v++){
				variatorList[v]->SetBase(base_);
			}
		}
		void CheckBranches() { 
			for(unsigned v = 0; v < variatorList.size(); v++){
				variatorList[v]->CheckBranches();
			}
		}
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
		KLooper* looper;
		vector<KVariator*> variatorList;
		KBase* base;
};

#endif
