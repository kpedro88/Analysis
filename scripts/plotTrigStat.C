#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"

#include "KCode/KParser.h"
#include "KCode/KPlot.h"
#include "KCode/KMap.h"
#include "KCode/KMath.h"
#include "corrections/TriggerFuncCorrector.h"

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <cmath>
#include <iostream>

using namespace std;

void plotTrigStat(string year){
	map<string,vector<string>> files{
		{"2016",{"corrections/trigEffFit_SingleMuon_2016_DetaHLTmatch.root","test/datacard_data_2016.root"}},
		{"2017",{"corrections/trigEffFit_SingleMuon_2017_DetaHLTmatch.root","test/datacard_data_2017.root"}},
		{"2018",{"corrections/trigEffFit_SingleMuon_2018_DetaHLTmatch.root","test/datacard_data_2018.root"}},
	};

	TriggerFuncCorrector trigfncorror;
	trigfncorror.SetFunc(files[year][0],"fit_MTAK8","err_MTAK8");

	vector<string> regions{"highCut","lowCut","highSVJ2","lowSVJ2"};
	auto dc_file = KOpen(files[year][1]);
	vector<TH1F*> hists;
	for(const auto& region: regions){
		hists.push_back(KGet<TH1F>(dc_file, region+"_2018/data_obs"));
	}

	//regions, then trig
	vector<Color_t> colors{kBlack, kBlue, kMagenta, kRed, kCyan};
	vector<int> markers{22, 20, 23, 21, 33};
	vector<double> x;
	vector<vector<double>> y(regions.size());
	vector<double> ytrig, yerr; //only for trig
	double ymin = 0;
	double ymax = 0;
	for(int b = 1; b <= hists[0]->GetNbinsX(); ++b){
		int ctr = 0;
		for(auto hist: hists){
			//relative statistical uncertainty
			y[ctr].push_back(1.0/KMath::PoissonErrorUp(hist->GetBinContent(b)));
			ymax = max(ymax,y[ctr].back());
			++ctr;
		}
		x.push_back(hists[0]->GetBinCenter(b));
		//size of deviation from 1
		double trigval = trigfncorror.GetCorrection(x.back(),0);
		ytrig.push_back(1-trigval);
		ymax = max(ymax,ytrig.back());
		yerr.push_back(max(abs(trigval-trigfncorror.GetCorrection(x.back(),-1)),abs(trigval-trigfncorror.GetCorrection(x.back(),1))));
	}
	KParser::printvec(ytrig,cout,","); cout << endl;

	OptionMap* plotOpt = new OptionMap();
	plotOpt->Set<string>("lumi_text",year+" (13 TeV)");
	plotOpt->Set<string>("prelim_text","");

	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	localOpt->Set<bool>("logy",false);
	localOpt->Set<int>("xnum",10);
	localOpt->Set<double>("xmin",hists[0]->GetXaxis()->GetXmin());
	localOpt->Set<double>("xmax",hists[0]->GetXaxis()->GetXmax());
	localOpt->Set<string>("xtitle","M_{T} [GeV]");
	localOpt->Set<string>("ytitle","relative uncertainty");

	string oname = "plotTrigStat"+year;
	auto plot = new KPlot(oname,localOpt,plotOpt);

	//make histo for axes
	bool init = plot->Initialize();
	if(!init) throw std::runtime_error("Plot failed to initialize, check options");
	auto hbase = plot->GetHisto();
	hbase->GetYaxis()->SetRangeUser(ymin,ymax*1.1);
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();
	//legend
	auto kleg = plot->GetLegend();
	kleg->AddHist(hbase); //for tick sizes

	//make graphs & add to legend
	vector<TGraph*> graphs;
	auto g_trig = new TGraphErrors(x.size(),x.data(),ytrig.data(),nullptr,yerr.data());
	g_trig->SetLineColor(colors.back());
	g_trig->SetMarkerColor(colors.back());
	g_trig->SetMarkerStyle(markers.back());
	kleg->AddEntry(g_trig, "1 - ^{}#varepsilon_{trig}", "pel");
	for(unsigned i = 0; i < regions.size(); ++i){
		graphs.push_back(new TGraph(x.size(),x.data(),y[i].data()));
		graphs.back()->SetLineColor(colors[i]);
		graphs.back()->SetMarkerColor(colors[i]);
		graphs.back()->SetMarkerStyle(markers[i]);
		kleg->AddEntry(graphs.back(), regions[i], "pl");
	}

	plot->DrawHist();
	//use graph "best" algo (must be after axes drawn)
	kleg->Build(KLegend::hdefault,KLegend::vdefault);
	//extra style options
	//auto leg = kleg->GetLegend();
	//leg->SetFillStyle(1001);
	//leg->SetFillColor(kGray);
	//leg->SetFillColorAlpha(0,0.6);

	//draw content
	g_trig->Draw("pz same");
	for(auto graph: graphs){
		graph->Draw("pz same");
	}
	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	kleg->Draw();
	plot->DrawText();

	can->Print((plot->GetName()+".png").c_str(),"png");
}
