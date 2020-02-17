#ifndef KFIT_H
#define KFIT_H

#include "KMap.h"
#include "KStyle.h"

#include <TF1.h>

#include <string>

using namespace std;

//fits can have their own styles, options, etc.
class KFit {
	public:
		KFit() : localOpt(new OptionMap()) {}
		KFit(string name_, OptionMap* localOpt_) : name(name_), localOpt(localOpt_ ? localOpt_ : new OptionMap()) {
			string fnname; localOpt->Get("fn",fnname);
			fn = new TF1(name.c_str(),fnname.c_str());
			vector<double> pars; localOpt->Get("pars",pars);
			fn->SetParameters(pars.data());
			freeze = localOpt->Get("freeze",false);
		}

		//accessors
		void SetStyle(OptionMap* defaultOpt){
			if(style) delete style;
			style = new KStyle(name, defaultOpt, localOpt);
			style->Format(fn);
		}
		void SetRange(double xmin, double xmax) { fn->SetRange(xmin, xmax); }
		TF1* GetFn() { return fn; }
		//option to normalize to yield of histo?

	protected:
		string name;
		OptionMap* localOpt;
		TF1* fn = nullptr;
		KStyle* style = nullptr;
		bool freeze = false;
};

#endif
