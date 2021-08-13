#include "KCode/KMap.h"
#include "KCode/KPlot.h"
#include "KCode/KLegend.h"

#include "TFile.h"
#include "TDirectory.h"
#include "TH1F.h"
#include "TPie.h"
#include "TPieSlice.h"

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <valarray>

using namespace std;

void makePieChart(string fname, string region){
	vector<string> bkgs{"QCD","TT","WJets","ZJets"};
	vector<int> cols{kBlue + 2, kOrange - 2, kOrange + 1, kViolet + 1};
	map<string,string> bdict{
		{"QCD","QCD"},
		{"TT","t#bar{t}"},
		{"WJets","W(l#nu)+jets"},
		{"ZJets","Z(#nu#nu)+jets"},
	};
	map<string,string> rdict{
		{"highCut","high-^{}R_{T}"},
		{"lowCut","low-^{}R_{T}"},
		{"highSVJ2","high-SVJ2"},
		{"lowSVJ2","low-SVJ2"},
	};

	string lumi_text("(13 TeV)");
	string prelim_text("Simulation Supplementary");

	TFile* file = KOpen(fname);
	TDirectory* dir = KGet<TDirectory>(file,region+"_2018");
	valarray<double> vals(bkgs.size());
	for(unsigned i = 0; i < bkgs.size(); ++i){
		auto h = KGet<TH1,TDirectory>(dir,bkgs[i]);
		vals[i] = h->Integral(0,h->GetNbinsX()+1);
	}
	//get proportions
	double val_sum = vals.sum();
	vals /= val_sum;

	TPie* pie = new TPie("pie","",bkgs.size(),&(vals[0]),cols.data());
	//label color
	pie->SetTextColor(kBlack);

	//setup plotting options
	OptionMap* plotOpt = new OptionMap();
	plotOpt->Set<string>("lumi_text",lumi_text);
	plotOpt->Set<string>("prelim_text",prelim_text);
	plotOpt->Set<bool>("checkerr",false);
	plotOpt->Set<int>("npanel",1);
	plotOpt->Set<double>("sizeLeg",20);
	plotOpt->Set<double>("canvasH",676);
	plotOpt->Set<double>("marginL",10);
	plotOpt->Set<double>("marginR",10);

	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	localOpt->Set<bool>("logy",false);

	string oname("pie_"+region);

	//fake histo
	TH1F* hbase = new TH1F("base","",1,0,1);
	hbase->GetYaxis()->SetRangeUser(0,1);
	KPlot* plot = new KPlot(oname,localOpt,plotOpt);
	plot->Initialize(hbase);
	KLegend* kleg = plot->GetLegend();
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();

	//fill legend
	kleg->AddEntry((TObject*)NULL,rdict[region],"");
	for(unsigned i = 0; i < bkgs.size(); ++i){
		stringstream ss;
		ss << fixed << setprecision(0);
		ss << vals[i]*100 << "%";
		pie->SetEntryLabel(i,(ss.str()).c_str());
		pie->SetEntryLabel(i,(ss.str()).c_str());
		auto slice = pie->GetSlice(i);
		kleg->AddEntry(slice,bdict[bkgs[i]],"f");
	}
	kleg->Build(KLegend::right,KLegend::top);

	//draw
	pie->Draw();
	plot->DrawText();

	for(const auto& pformat: {"png","pdf"}){
		can->Print((plot->GetName()+"."+pformat).c_str(),pformat);
	}
}