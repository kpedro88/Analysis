#include "TFile.h"
#include "TGraph.h"
#include "TLine.h"
#include "TAttFill.h"

#include "KCode/KPlot.h"
#include "KCode/KMap.h"

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <exception>
#include <map>

using namespace std;

pair<vector<double>,vector<double>> readSpikeList(const string& fname){
	pair<vector<double>,vector<double>> eta_phi_list;
	ifstream infile(fname.c_str());
	if(infile.is_open()){
		string line;
		while(getline(infile,line)){
			vector<string> fields;
			KParser::process(line,'\t',fields);
			eta_phi_list.first.emplace_back(KParser::getOptionValue<double>(fields[0]));
			eta_phi_list.second.emplace_back(KParser::getOptionValue<double>(fields[1]));
		}
	}
	else throw runtime_error("missing phi spike list: "+fname);
	return eta_phi_list;
}

void plotDeadCells(string year){
	map<string,vector<string>> files{
		{"2016",{"../jecsys/rootfiles/UL2016_badchannels.root","corrections/phispikes16.txt","corrections/leadphispikes16.txt"}},
		{"2017",{"../jecsys/rootfiles/UL2017_badchannels.root","corrections/phispikes17.txt","corrections/leadphispikes17.txt"}},
		{"2018",{"../jecsys/rootfiles/UL2018_badchannels.root","corrections/phispikes18.txt","corrections/leadphispikes18.txt"}},
	};

	auto jme_file = KOpen(files[year][0]);
	auto h_eb = KGet<TH2D>(jme_file,"ebmap_etaphi");
	auto g_ee = KGet<TGraph>(jme_file,"eemap_etaphi");
	g_ee->SetMarkerColor(kCyan);

	const auto& spikes = readSpikeList(files[year][1]);
	auto g_spikes = new TGraph(spikes.first.size(),spikes.first.data(),spikes.second.data());
	g_spikes->SetMarkerStyle(24);
	g_spikes->SetMarkerColor(kBlack);

	const auto& leadspikes = readSpikeList(files[year][2]);
	auto g_leadspikes = new TGraph(leadspikes.first.size(),leadspikes.first.data(),leadspikes.second.data());
	g_leadspikes->SetMarkerStyle(27);
	g_leadspikes->SetMarkerColor(kBlack);

	OptionMap* plotOpt = new OptionMap();
	plotOpt->Set<string>("lumi_text",year+" (13 TeV)");
	plotOpt->Set<string>("prelim_text","");

	double xmin = -3.5;
	double xmax = 3.5;
	double ymin = -3.1415926536;
	double ymax = 3.1415926536;
	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	localOpt->Set<bool>("logz",false);
	localOpt->Set<int>("xnum",10);
	localOpt->Set<double>("xmin",xmin);
	localOpt->Set<double>("xmax",xmax);
	localOpt->Set<string>("xtitle","#eta");
	localOpt->Set<int>("ynum",10);
	localOpt->Set<double>("ymin",ymin);
	localOpt->Set<double>("ymax",ymax);
	localOpt->Set<string>("ytitle","#phi");

	string oname = "plotDeadCells"+year;
	auto plot = new KPlot2D(oname,"",localOpt,plotOpt);

	//make histo for axes
	bool init = plot->Initialize();
	if(!init) throw std::runtime_error("Plot failed to initialize, check options");
	auto hbase = plot->GetHisto();
	hbase->GetZaxis()->SetRangeUser(h_eb->GetMinimum(),h_eb->GetMaximum());
	hbase->GetZaxis()->SetTitle("");
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();

	//denote analysis boundaries
	double etamin = -2.4;
	double etamax = 2.4;
	auto line_p = new TLine(etamax,ymin,etamax,ymax);
	line_p->SetLineStyle(7);
	line_p->SetLineWidth(2);
	line_p->SetLineColor(kBlack);
	auto line_m = new TLine(etamin,ymin,etamin,ymax);
	line_m->SetLineStyle(7);
	line_m->SetLineWidth(2);
	line_m->SetLineColor(kBlack);

	//legend
	KLegend* kleg = new KLegend(pad1,localOpt,plotOpt);
	kleg->AddHist(hbase); //for tick sizes
	auto box = new TH1F("box","",1,xmin,xmax);
	box->SetFillColor(kRed);
	kleg->AddEntry(box,"EB","f");
	kleg->AddEntry(g_ee,"EE","p");
	kleg->AddEntry(g_spikes,"spikes (^{}j_{2})","p");
	kleg->AddEntry(g_leadspikes,"spikes (^{}j_{1})","p");
	kleg->AddEntry(line_p,"analysis cut","l",0,{"(|^{}#eta_{J}| < 2.4)"});

	plot->DrawHist();
	//use graph "best" algo (must be after axes drawn)
	kleg->Build(KLegend::hdefault,KLegend::vdefault);
	//extra style options
	auto leg = kleg->GetLegend();
	leg->SetFillStyle(1001);
	leg->SetFillColor(kGray);
	leg->SetFillColorAlpha(0,0.6);

	//draw content
	h_eb->Draw("colz same");
	g_ee->Draw("pz same");
	g_spikes->Draw("pz same");
	g_leadspikes->Draw("pz same");
	line_p->Draw("same");
	line_m->Draw("same");
	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	kleg->Draw();
	plot->DrawText();

	can->Print((plot->GetName()+".png").c_str(),"png");
}
