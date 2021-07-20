#include "KCode/KCutflow.h"
#include "KCode/KMap.h"
#include "KCode/KMath.h"

#include <TFile.h>
#include <TH1.h>

#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

struct Sample {
	Sample(string name_, double xsec_, double lumi_, string name2_="") : name(name_), name2(!name2_.empty() ? name2_ : name), xsec(xsec_), lumi(lumi_) {}
	string name, name2;
	double xsec, lumi;
};

struct SignalRegion {
	SignalRegion(string name_) : name(name_), raw(0), rawE2(0) {}
	string name;
	double raw, rawE2;
};

void CutflowSum(string dir="", vector<Sample> samples={}, bool printerrors=false, int prcsn=2, bool skipzeros=false, string srname="", vector<string> regions={}){
	if(dir.empty()){
		cout << "Recompiled CutflowSum, exiting." << endl;
		return;
	}

	TFile* sfile = NULL;
	vector<SignalRegion> syields;
	//include signal regions: for SVJ
	if(!srname.empty() and !regions.empty()){
		sfile = KOpen(srname);
		for(const auto& region: regions){
			syields.emplace_back(region);
		}
	}

	TH1F* nEventHist = NULL;
	TH1F* cutflowRaw = NULL;

	for(const auto& sample : samples){
		TFile* file = KOpen(dir+"/tree_"+sample.name+".root");
		
		//get histos
		auto nEventTmp = KGet<TH1F>(file,"nEventProc");
		nEventTmp->SetBinError(1,KMath::PoissonErrorUp(nEventTmp->GetBinContent(1)));
		auto cutflowTmp = KGet<TH1F>(file,"cutflow");

		//fix error values: PoissonErrorUp doesn't hadd correctly
		for(int b = 1; b <= cutflowTmp->GetNbinsX(); ++b){
			if(skipzeros and cutflowTmp->GetBinContent(b)==0.) cutflowTmp->SetBinError(b,0.);
			else cutflowTmp->SetBinError(b,KMath::PoissonErrorUp(cutflowTmp->GetBinContent(b)));
		}

		//scale histos
		double weight = sample.lumi * sample.xsec / nEventTmp->GetBinContent(1);
		nEventTmp->Scale(weight);
		cutflowTmp->Scale(weight);
		
		if(nEventHist==NULL){
			nEventHist = (TH1F*)nEventTmp->Clone(); nEventHist->SetDirectory(0);
			cutflowRaw = (TH1F*)cutflowTmp->Clone(); cutflowRaw->SetDirectory(0);
		}
		else {
			nEventHist->Add(nEventTmp);
			cutflowRaw->Add(cutflowTmp);
		}

		for(auto& syield: syields){
			auto dir = KGet<TDirectory>(sfile,syield.name);
			auto htmp = KGet<TH1F>(dir,sample.name2);
			double integral = htmp->Integral(-1,-1);
			syield.raw += integral*weight;
			syield.rawE2 += pow((integral>0 ? KMath::PoissonErrorUp(integral) : 0.)*weight,2);
		}
	}

	cout << fixed << setprecision(prcsn);
	KCutflow k("print",cutflowRaw,nEventHist,true);

	//add signal regions
	const auto& parent = k.GetList().back()->GetName();
	for(const auto& syield: syields){
		k.AddItem(syield.name, syield.raw, sqrt(syield.rawE2), KCutflow::nEventProc(), parent);
	}

	k.PrintEfficiency(printerrors);
}
