#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>

#include "KCode/KMap.h"
#include "KCode/KPlot.h"
#include "KCode/KLegend.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace std;

//usage:
//root -l 'plotBTagEff.C+("test/tree_signalBtag/tree_T1tttt_1500_100_btagSF.root","T1tttt_1500_100")'

void plotBTagEff(string fname, string osuff, bool applySF=true){
	TFile* file = TFile::Open(fname.c_str(),"READ");
	
	//get asymm graphs
	TGraphAsymmErrors* a_eff_b;
	a_eff_b = (TGraphAsymmErrors*)file->Get("a_eff_b_central");
	TGraphAsymmErrors* a_eff_b_unc = NULL;
	if(applySF){
		TGraphAsymmErrors* a_eff_b_up = (TGraphAsymmErrors*)file->Get("a_eff_b_up");
		TGraphAsymmErrors* a_eff_b_down = (TGraphAsymmErrors*)file->Get("a_eff_b_down");
		//make syst band
		a_eff_b_unc = (TGraphAsymmErrors*)a_eff_b->Clone("a_eff_b_unc");
		for(int b = 0; b < a_eff_b_unc->GetN(); b++){
			double x, y;
			a_eff_b_unc->GetPoint(b,x,y);
			double x_up, y_up;
			a_eff_b_up->GetPoint(b,x_up,y_up);
			double x_down, y_down;
			a_eff_b_down->GetPoint(b,x_down,y_down);
			
			//cout << y - y_down << ", " << y_up - y << endl;
			a_eff_b_unc->SetPointEYlow(b, y - y_down);
			a_eff_b_unc->SetPointEYhigh(b, y_up - y);
		}
	}
	
	//formatting
	a_eff_b->SetLineColor(kBlue);
	a_eff_b->SetMarkerColor(kBlue);
	a_eff_b_unc->SetFillColor(kGray+3);
	a_eff_b_unc->SetFillStyle(3005);
	
	//get corresponding histo for legend placement
	TH1* htmp = (TH1*)file->Get("h_eff_b");
	vector<double> xbins(htmp->GetXaxis()->GetXbins()->GetArray(),htmp->GetXaxis()->GetXbins()->GetArray()+htmp->GetNbinsX()+1);
	
	//setup plotting options
	OptionMap* globalOpt = new OptionMap();
	//globalOpt->Set<string>("prelim_text","Simulation");
	globalOpt->Set<string>("prelim_text","Supplementary");
	globalOpt->Set<string>("lumi_text","(13 TeV)");
	globalOpt->Set<bool>("checkerr",false);
	globalOpt->Set<double>("sizeLeg",21.5);
	globalOpt->Set<double>("sizeSymb",0.1);

	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	localOpt->Set<bool>("logy",false);
	localOpt->Set<bool>("logx",true);
	localOpt->Set<bool>("morelogxlabels",true);
	localOpt->Set<vector<double> >("xbins",xbins);
	localOpt->Set<string>("xtitle","p_{T} [GeV]");
	localOpt->Set<string>("ytitle","b-tagging efficiency");
	
	//make plot
	string oname = "effb_"+osuff;
	KPlot* plot = new KPlot(oname,localOpt,globalOpt);
	plot->Initialize();
	KLegend* kleg = plot->GetLegend();
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();
	
	//draw blank histo for axes
	plot->DrawHist();
	
	//build legend
	plot->GetHisto()->GetYaxis()->SetRangeUser(0,0.85);
	kleg->AddHist(plot->GetHisto()); //for tick sizes
	kleg->AddHist(htmp); //for legend alignment
	kleg->AddEntry(a_eff_b,"pp #rightarrow #tilde{g}#tilde{g}, #tilde{g} #rightarrow t#bar{t} ^{}#tilde{#chi}^{0}_{1} (^{}m_{#tilde{g}} = 1500 GeV, ^{}m_{^{}#tilde{#chi}^{0}_{1}} = 100 GeV)","pel");
	if(a_eff_b_unc) kleg->AddEntry(a_eff_b_unc,"systematic uncertainty","f");
	kleg->AddEntry((TObject*)NULL,"#color[4]{arXiv:1602.06581}","");
	kleg->Build();
	
	//draw central value and uncertainty
	if(a_eff_b_unc) a_eff_b_unc->Draw("2 same");
	a_eff_b->Draw("pz same");
	
	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();

	//print image
	string printformats[] = {"png","eps"};
	for(unsigned p = 0; p < 2; ++p){
		can->Print((oname+"."+printformats[p]).c_str(),printformats[p].c_str());
	}
	system(("epstopdf "+oname+".eps").c_str());
	
	//print latex table
	cout << fixed << setprecision(3);
	for(int b = 0; b < a_eff_b->GetN(); b++){
		double x, y;
		a_eff_b->GetPoint(b,x,y);
		
		double yEstat = 0, yEsyst = 0;
		yEstat = a_eff_b->GetErrorYhigh(b);
		if(a_eff_b_unc) yEsyst = a_eff_b_unc->GetErrorYhigh(b);
		
		cout << (int)(xbins[b]) << " & " << (int)(xbins[b+1]) << " & " << y << " & " << yEstat;
		if(a_eff_b_unc) cout << " & " << yEsyst;
		cout << " \\\\" << endl;
	}
}