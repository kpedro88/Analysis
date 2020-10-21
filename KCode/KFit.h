#ifndef KFIT_H
#define KFIT_H

#include "KMap.h"
#include "KStyle.h"
#include "KLegend.h"

#include <TF1.h>
#include <TFitResultPtr.h>
#include <TH1.h>
#include <TPad.h>

#include <string>
#include <iomanip>
#include <sstream>
#include <exception>
#include <algorithm>
#include <vector>

using namespace std;

//fits can have their own styles, options, etc.
class KFit {
	public:
		KFit() : localOpt(new OptionMap()) {}
		KFit(string name_, OptionMap* localOpt_) : name(name_), localOpt(localOpt_ ? localOpt_ : new OptionMap()) {
			string fnname; localOpt->Get("fn",fnname);
			//supports constructors of type: 2 - Expression using variable x with parameters
			fn = new TF1(name.c_str(),fnname.c_str());
			double xmin, xmax;
			setrange = localOpt->Get("xmin",xmin) and localOpt->Get("xmax",xmax);
			if(setrange) fn->SetRange(xmin,xmax);
			vector<double> pars;
			if(localOpt->Get("pars",pars)){
				//safety check
				if(pars.size()!=unsigned(fn->GetNpar())) throw runtime_error("Inconsistent size of pars ("+to_string(pars.size())+") vs. fit ("+to_string(fn->GetNpar())+")");
				fn->SetParameters(pars.data());
			}
			//options for TH1::Fit()
			localOpt->Get("opts",opts);
			opts += "N";
			legname = name;
			localOpt->Get("legname",legname);
			localOpt->Get("splitleg",splitleg);
			//freeze specific or all parameters (freeze all = don't fit at all)
			vector<int> fixpars; localOpt->Get("fixpars",fixpars);
			bool freeze = localOpt->Get("freeze",false);
			if(freeze) {
				fixpars.clear(); fixpars.resize(pars.size(),0);
				iota(fixpars.begin(),fixpars.end(),0);
			}
			for(auto fp : fixpars){
				fn->FixParameter(fp,fn->GetParameter(fp));
			}
			//parameter for normalization
			localOpt->Get("normpar",normpar);
			//precision for printing pars and chi2
			localOpt->Get("precision",prcsn);
			precfixed = localOpt->Get("precfixed",true);
			printerr = localOpt->Get("printerr",false);
		}

		//accessors
		void SetStyle(const KStyle& orig){
			if(style) delete style;
			style = new KStyle(orig);
			style->SetLocalOpt(localOpt);
			style->Format(fn);
		}
		void SetRange(double xmin, double xmax) {
			//skip this if range already set by input
			if(!setrange) fn->SetRange(xmin, xmax);
		}
		TF1* GetFn() { return fn; }
		const string& GetOpts() { return opts; }
		KStyle* GetStyle() { return style; }
		string GetName() { return name; }
		//fit and/or normalize (if requested)
		template <typename T>
		void DoFit(T* htmp) {
			Normalize(htmp);
			//if all pars are fixed, this just sets chi2 and ndf
			ptr = htmp->Fit(fn,opts.c_str());
		}
		template <typename T>
		void Normalize(T* htmp) {}
		//todo: add option to plot fit error band?
		void AddToLegend(KLegend* kleg, int panel=0, bool assoc=false){
			stringstream sleg;
			if(precfixed) sleg << fixed;
			sleg << setprecision(prcsn);
			sleg << legname << ", #chi^{2} / n_{dof} = " << fn->GetChisquare() << " / " << fn->GetNDF();
			string ssleg = sleg.str();
			vector<string> extra_text;
			//print params 2 at a time to avoid overflow
			int ctr = 0; sleg.str("");
			for(int i = 0; i < fn->GetNpar(); ++i){
				sleg << "p_{" << i << "} = " << fn->GetParameter(i);
				if(printerr) sleg << " #pm " << fn->GetParError(i);
				if(ctr==splitleg-1 or i==fn->GetNpar()-1){
					extra_text.push_back(sleg.str());
					sleg.str("");
					ctr = 0;
				}
				else {
					sleg << ", ";
					++ctr;
				}
			}
			string option = style->GetLegOpt();
			kleg->AddEntry(fn,ssleg,option,panel,extra_text,assoc);
		}
		void Draw(TPad* pad){
			fn->Draw(style->GetDrawOpt("same").c_str());
		}

	protected:
		string name;
		OptionMap* localOpt;
		TF1* fn = nullptr;
		TFitResultPtr ptr;
		KStyle* style = nullptr;
		bool setrange = false;
		bool freeze = false;
		string legname;
		string opts;
		int splitleg = 2;
		int normpar = -1;
		int prcsn = 2;
		bool precfixed = true;
		bool printerr = false;
};

//only normalize to histos
template <>
void KFit::Normalize<TH1>(TH1* htmp) {
	if(normpar >= 0) {
		//set normpar to 1 to get function integral
		fn->SetParameter(normpar,1.);
		double int_f = fn->Integral(htmp->GetBinLowEdge(1),htmp->GetBinLowEdge(htmp->GetNbinsX()+1));
		//get function integral and histo integral over same range
		double int_h = htmp->Integral(1,htmp->GetNbinsX());
		fn->FixParameter(normpar,int_h/int_f);
	}
}

#endif
