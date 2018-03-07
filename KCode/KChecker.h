#ifndef KCHECKER_H
#define KCHECKER_H

//custom headers
#include "KMap.h"
#include "KBase.h"

//STL headers
#include <string>

using namespace std;

//forward declarations
class KBase;
class KLooper;
class KSelection;

//-----------------------------------------------------------------
//base for classes that check content of KBase, KLooper, KSelection
//and that have their own name and option map
class KChecker {
	public:
		//constructors
		KChecker() : name(""), localOpt(new OptionMap()), base(0), looper(0), sel(0) {}
		KChecker(string name_, OptionMap* localOpt_) : name(name_), localOpt(localOpt_ ? localOpt_ : new OptionMap()), base(0), looper(0), sel(0) {}
		
		//destructor
		virtual ~KChecker() {}
		
		//accessors
		string GetName() { return name; }
		virtual void SetSelection(KSelection* sel_) { sel = sel_; CheckDeps(); } //set dependencies here if desired
		virtual void SetBase(KBase* base_); //implemented in KBase.h to avoid circular dependency
		
		//to check dependencies on other selectors
		virtual void CheckDeps() {}
		//to allow selectors to turn on branches they need
		virtual void CheckBranches() {}
		//to check dependencies on the base (and the looper)
		virtual void CheckBase() {}
		
	protected:
		//member variables
		string name;
		OptionMap* localOpt;
		KBase* base;
		KLooper* looper;
		KSelection* sel;
};

#endif