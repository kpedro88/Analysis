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

using namespace std;

struct Sample {
	Sample(string name_, double xsec_, double lumi_) : name(name_), xsec(xsec_), lumi(lumi_) {}
	string name;
	double xsec, lumi;
};

void CutflowSum(string dir="", vector<Sample> samples={}, bool printerrors=false, int prcsn=2, bool skipzeros=false){
	if(dir.empty()){
		cout << "Recompiled CutflowSum, exiting." << endl;
		return;
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
	}
	
	cout << fixed << setprecision(prcsn);
	KCutflow k("print",cutflowRaw,nEventHist,true);
	k.PrintEfficiency(printerrors);
}
