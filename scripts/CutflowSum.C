#include "KCode/KCutflow.h"
#include "KCode/KMap.h"

#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <iomanip>

using namespace std;

void CutflowSum(string dir="", string sample="", vector<pair<string,double>> yearweights={}, bool printerrors=false, int prcsn=2){
	if(dir.empty()){
		cout << "Recompiled CutflowSum, exiting." << endl;
		return;
	}
	
	TH1F* nEventHist = NULL;
	TH1F* cutflowRaw = NULL;
	
	for(const auto& yearweight : yearweights){
		auto year = yearweight.first;
		auto weight = yearweight.second;
		TFile* file = KOpen(dir+"/tree_"+sample+"_MC"+year+"_fast.root");
		
		//get and scale histos
		auto nEventTmp = KGet<TH1F>(file,"nEventProc");
		auto cutflowTmp = KGet<TH1F>(file,"cutflow");
		weight /= nEventTmp->GetBinContent(1);
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
	KCutflow k("print",cutflowRaw,nEventHist);
	k.PrintEfficiency(printerrors);
}
