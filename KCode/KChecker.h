#ifndef KCHECKER_H
#define KCHECKER_H

//custom headers
#include "KMap.h"
#include "KLooper.h"

//STL headers
#include <string>
#include <vector>

using namespace std;

//forward declarations
class KBase;
class KSelection;

//-----------------------------------------------------------------
//base for classes that check content of KBase, KLooper, KSelection
//and that have their own name and option map
class KChecker {
	public:
		//constructors
		KChecker() : name(""), orig_name(""), localOpt(new OptionMap()), base(0), looper(0), sel(0) {}
		KChecker(string name_, OptionMap* localOpt_)
			: name(name_), orig_name(name_), localOpt(localOpt_ ? localOpt_ : new OptionMap()), base(0), looper(0), sel(0)
		{
			//possible to change name from default
			localOpt->Get("name",name);
		}
		
		//destructor
		virtual ~KChecker() {}
		
		//accessors
		string GetName() { return name; }
		virtual void SetSelection(KSelection* sel_) { sel = sel_; CheckDeps(); } //set dependencies here if desired
		virtual void SetBase(KBase* base_); //implemented in KBase.h to avoid circular dependency
		
		//to check dependencies on other selectors
		virtual void CheckDeps() {}
		//to allow selectors to turn on branches they need
		virtual void CheckBranches() {
			ListBranches(); //fills branches vector
			looper->EnableBranches(branches);
		}
		virtual void ListBranches() {}
		//to check dependencies on the base (and the looper)
		virtual void CheckBase() {}
		
	protected:
		//member variables
		string name, orig_name;
		OptionMap* localOpt;
		KBase* base;
		KLooper* looper;
		KSelection* sel;
		vector<string> branches;
};

#endif
