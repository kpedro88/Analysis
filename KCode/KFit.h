#ifndef KFIT_H
#define KFIT_H

#include "KMap.h"
#include "KMath.h"
#include "KStyle.h"
#include "KLegend.h"

#include <TF1.h>
#include <TFitResultPtr.h>
#include <TH1.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TPad.h>

#include <string>
#include <iomanip>
#include <sstream>
#include <exception>
#include <algorithm>
#include <vector>
#include <numeric>

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
			altchi2 = localOpt->Get("altchi2",false);
			legname = name;
			localOpt->Get("legname",legname);
			legpars = localOpt->Get("legpars",true);
			splitchi2 = localOpt->Get("splitchi2",false);
			localOpt->Get("splitleg",splitleg);
			//freeze specific or all parameters (freeze all = don't fit at all)
			vector<int> fixpars; localOpt->Get("fixpars",fixpars);
			freeze = localOpt->Get("freeze",false);
			if(freeze) {
				fixpars.clear(); fixpars.resize(pars.size(),0);
				iota(fixpars.begin(),fixpars.end(),0);
			}
			for(auto fp : fixpars){
				fn->FixParameter(fp,fn->GetParameter(fp));
			}
			//parameter for normalization
			localOpt->Get("normpar",normpar);
			use_yield = localOpt->Get("yield",yield);
			//precision for printing pars and chi2
			localOpt->Get("precision",prcsn);
			precfixed = localOpt->Get("precfixed",true);
			printerr = localOpt->Get("printerr",false);

			//error band (optional)
			string bandfilename, bandname;
			bool has_band = localOpt->Get("bandfile",bandfilename) and localOpt->Get("bandname",bandname);
			if(has_band){
				auto bandfile = KOpen(bandfilename);
				band = KGet<TGraph>(bandfile,bandname);
				bstyle = KStyle::GetWithDefault("band",localOpt,false,"g");
				bstyle->Format(band);
				band->SetName(("errorband_"+name).c_str());
			}
			//todo: add option to get error band from fit directly (alternative to importing)
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
		TGraph* GetBand() { return band; }
		const string& GetOpts() { return opts; }
		KStyle* GetStyle() { return style; }
		string GetName() { return name; }
		//fit and/or normalize (if requested)
		template <typename T>
		void DoFit(T* htmp) {
			Normalize(htmp);
			//if all pars are fixed, this just sets chi2 and ndf
			ptr = htmp->Fit(fn,opts.c_str());
			//if all fixed: adjust ndf (exclude normpar)
			if(freeze) fn->SetNDF(fn->GetNDF()-fn->GetNpar()+(normpar>=0?1:0));
			if(altchi2) DoAltChi2(htmp);
		}
		template <typename T>
		void DoAltChi2(T* htmp) {}
		template <typename T>
		void Normalize(T* htmp) {}
		void AddToLegend(KLegend* kleg, int panel=0, bool assoc=false){
			stringstream sleg, schi2;
			if(precfixed) sleg << fixed;
			sleg << legname;
			schi2 << setprecision(prcsn);
			schi2 << "^{}#chi^{2} / ^{}n_{dof} = " << fn->GetChisquare() << " / " << fn->GetNDF();
			if(!splitchi2) sleg << ", " << schi2.str();
			string ssleg = sleg.str();
			vector<string> extra_text;
			localOpt->Get("extra_text",extra_text);
			if(splitchi2) extra_text.push_back(schi2.str());
			if(legpars){
				//print params 2 at a time to avoid overflow
				int ctr = 0; sleg.str("");
				for(int i = 0; i < fn->GetNpar(); ++i){
					sleg << "^{}p_{" << i << "} = " << fn->GetParameter(i);
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
			}
			string option = style->GetLegOpt();
			kleg->AddEntry(fn,ssleg,style->GetLegOpt(),panel,extra_text,assoc);
			if(band) kleg->AddEntry(band,"uncertainty [^{}"+legname+"]",bstyle->GetLegOpt(),panel,{},assoc);
		}
		void Draw(TPad* pad){
			if(band) band->Draw(bstyle->GetDrawOpt("same").c_str());
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
		bool legpars;
		string opts;
		bool altchi2;
		bool splitchi2;
		int splitleg = 2;
		int normpar = -1;
		bool use_yield;
		double yield;
		int prcsn = 2;
		bool precfixed = true;
		bool printerr = false;
		TGraph* band = nullptr;
		KStyle* bstyle = nullptr;
};

//only normalize to histos
template <>
void KFit::Normalize<TH1>(TH1* htmp) {
	if(normpar >= 0) {
		//set normpar to 1 to get function integral
		fn->SetParameter(normpar,1.);
		double int_f = fn->Integral(htmp->GetBinLowEdge(1),htmp->GetBinLowEdge(htmp->GetNbinsX()+1));
		//get function integral and histo integral over same range (unless manual yield given)
		double int_h = use_yield ? yield : htmp->Integral(1,htmp->GetNbinsX(),"width");
		fn->FixParameter(normpar,int_h/int_f);
	}
}

//only implemented for TGraphAsymmErrors
template <>
void KFit::DoAltChi2(TGraphAsymmErrors* gtmp){
	//copy the computation from RooCurve::chiSquare()
	double chisq = 0.;
	for(int i = 0; i < gtmp->GetN(); ++i){
		double avg = fn->Integral(gtmp->GetX()[i]-gtmp->GetEXlow()[i], gtmp->GetX()[i]+gtmp->GetEXhigh()[i])/(gtmp->GetEXlow()[i]+gtmp->GetEXhigh()[i]);
		double y = gtmp->GetY()[i];
		if(y!=0){
			double err = (y>avg) ? gtmp->GetEYlow()[i] : gtmp->GetEYhigh()[i];
			double pull = (y-avg)/err;
			chisq += pull*pull;
		}
	}
	fn->SetChisquare(chisq);
}

//convert histogram to TGraphAsymmErrors
//use Poisson error calculation if kPoisson error option is enabled
//(this error option doesn't actually work all the time in ROOT 6.06)
template <>
void KFit::DoAltChi2(TH1* htmp){
	bool poisson = htmp->GetBinErrorOption()==TH1::kPoisson;
	//for this to be completely correct, kPoisson error option should be set for histo
	if(!poisson) cout << "Warning: using RooFit chi2 computation with Gaussian errors for fit " << name << endl;
	vector<vector<double>> values(5); //x, ex, y, eyl, eyh
	for(auto& v : values){ v.reserve(htmp->GetNbinsX()); }
	for(int i = 1; i < htmp->GetNbinsX(); ++i){
		auto axis = htmp->GetXaxis();
		values[0].push_back(axis->GetBinCenter(i));
		values[1].push_back(axis->GetBinWidth(i)/2.);
		values[2].push_back(htmp->GetBinContent(i));
		values[3].push_back(poisson ? KMath::PoissonErrorLow(values[2].back()) : htmp->GetBinError(i));
		values[4].push_back(poisson ? KMath::PoissonErrorUp(values[2].back()) : htmp->GetBinError(i));
	}
	TGraphAsymmErrors* gtmp = new TGraphAsymmErrors(
		values[0].size(),
		values[0].data(), values[2].data(), values[1].data(),
		values[1].data(), values[3].data(), values[4].data()
	);
	DoAltChi2(gtmp);
}

#endif
