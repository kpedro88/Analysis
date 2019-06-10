#include "Helper.h"

#include <TFile.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>

#include <vector>
#include <string>
#include <cmath>

void convertTGAEtoTH2(){
	TFile* infile = helper::Open("Tracking_EfficienciesAndSF_BCDEFGH.root");
	
	std::vector<std::string> objs = {"ratio_eff_eta3_tk0_dr030e030_corr","ratio_eff_eta3_dr030e030_corr"};
	
	std::vector<TH2*> hists;
	
	for(const auto& obj: objs){
		TGraphAsymmErrors* graph = helper::Get<TGraphAsymmErrors>(infile,obj);
		std::vector<double> bins;
		for(unsigned b = 0; b < graph->GetN(); ++b){
			bins.push_back(graph->GetX()[b]-graph->GetErrorXlow(b));
		}
		bins.push_back(graph->GetX()[graph->GetN()-1]+graph->GetErrorXhigh(graph->GetN()-1));
		//make a fake pt bin
		TH2* htmp = new TH2F(obj.c_str(),"",bins.size()-1,bins.data(),1,0,1000);
		for(unsigned b = 1; b < bins.size(); ++b){
			htmp->SetBinContent(b,1,graph->GetY()[b-1]);
			htmp->SetBinError(b,1,std::max(graph->GetErrorYlow(b-1),graph->GetErrorYhigh(b-1)));
		}
		hists.push_back(htmp);
	}
	
	TFile* outfile = helper::Open("Tracking_EfficienciesAndSF_BCDEFGH_TH2.root","RECREATE");
	outfile->cd();
	for(auto h : hists){
		h->Write();
	}
	outfile->Close();
}
