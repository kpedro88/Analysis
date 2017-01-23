#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2Poly.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TList.h>
#include <TCollection.h>
#include <TMath.h>

#include "KCode/KMap.h"
#include "KCode/KPlot.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

//usage:
//root -l plotHTMHT.C+

void plotHTMHT(bool do_QCD=false){
	//define bin ranges
	const float inf_HT = 2100;
	const float inf_MHT = 1000;
	int nbins = 10;
	if(do_QCD) nbins = 13;
	string bin_name[] = {"1","2","3","4","5","6","7","8","9","10","C1","C2","C3"};
	float bin_min_HT[] = {300,500,1000,350,500,1000,500,1000,750,1500,300,500,1000};
	float bin_max_HT[] = {500,1000,inf_HT,500,1000,inf_HT,1000,inf_HT,1500,inf_HT,500,1000,inf_HT};
	float bin_min_MHT[] = {300,300,300,350,350,350,500,500,750,750,250,250,250};
	float bin_max_MHT[] = {350,350,350,500,500,500,750,750,inf_MHT,inf_MHT,300,300,300};
	
	//define skipped bins
	const int nbinsSkip = 3;
	float binSkip_min_HT[] = {300,300,300};
	float binSkip_max_HT[] = {350,500,750};
	float binSkip_min_MHT[] = {350,500,750};
	float binSkip_max_MHT[] = {500,750,inf_MHT};
	
	//create base histo
	TH1F* hist = new TH1F("bkg","",1,*min_element(bin_min_HT,bin_min_HT+nbins),inf_HT);
	hist->GetYaxis()->SetRangeUser(*min_element(bin_min_MHT,bin_min_MHT+nbins),inf_MHT);
	//hist->SetBinContent(1,inf_MHT);
	hist->GetXaxis()->SetTitle("H_{T} [GeV]");
	hist->GetYaxis()->SetTitle("H_{T}^{miss} [GeV]");
	
	//create plane
	TH2Poly* hpoly = new TH2Poly();
	hpoly->SetTitle("");
	hpoly->SetLineColor(kBlack);
	hpoly->SetLineWidth(3);
	hpoly->SetFillStyle(1001); //solid
	hpoly->SetFillColor(kWhite);	
	gStyle->SetPaintTextFormat("g");
	
	//create bins
	for(unsigned b = 0; b < nbins; ++b){
		int thebin = hpoly->AddBin(bin_min_HT[b],bin_min_MHT[b],bin_max_HT[b],bin_max_MHT[b]);
		//skip filling
		//hpoly->Fill((bin_min_HT[b]+bin_max_HT[b])/2.,(bin_min_MHT[b]+bin_max_MHT[b])/2.,bin_num[b]);
	}
	
	//create skipped bins
	TH2Poly* hskip = new TH2Poly();
	for(unsigned b = 0; b < nbinsSkip; ++b){
		int thebin = hskip->AddBin(binSkip_min_HT[b],binSkip_min_MHT[b],binSkip_max_HT[b],binSkip_max_MHT[b]);
		hskip->Fill((binSkip_min_HT[b]+binSkip_max_HT[b])/2.,(binSkip_min_MHT[b]+binSkip_max_MHT[b])/2.);
	}
	
	//setup plotting options
	OptionMap* globalOpt = new OptionMap();
	//globalOpt->Set<string>("prelim_text","Simulation");
	globalOpt->Set<double>("marginL",110);
	globalOpt->Set<double>("NdivX",-6);
	globalOpt->Set<double>("NdivYhisto",10);

	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	localOpt->Set<bool>("logy",false);
	localOpt->Set<bool>("logx",false);
	
	//make plot
	string oname = "HT-MHT-2016";
	if(do_QCD) oname += "-QCD";
	KPlot* plot = new KPlot(oname,localOpt,globalOpt);
	plot->Initialize(hist);
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();
	
	//draw blank histo for axes
	gStyle->SetFrameLineWidth(2);
	hist->Draw("hist");
	
	//draw skipped boxes
	{
		TList *bins = hskip->GetBins(); TIter Next(bins);
		TObject *obj;
		while ((obj = Next())) {
			TH2PolyBin* b = (TH2PolyBin*)obj;
			TGraph* g = (TGraph*)b->GetPolygon();
			if (g) {
				g->SetFillStyle(3013); //cross-hatched
				g->SetFillColor(kBlack);
				g->Draw("f same");
			}
		}
	}
	
	//draw poly w/ text
	//stolen from: hist/histpainter/src/THistPainter.cxx: void THistPainter::PaintTH2PolyText()
	{
		TList *bins = hpoly->GetBins(); TIter Next(bins);
		TObject *obj;
		int counter = 0;
		while ((obj = Next())) {
			TH2PolyBin* b = (TH2PolyBin*)obj;
			TGraph* g = (TGraph*)b->GetPolygon();
			if (g) {
				if(bin_name[counter]=="C1" or bin_name[counter]=="1" or bin_name[counter]=="4"){
					g->SetFillStyle(3345);
					g->SetFillColor(kAzure+10);
					g->Draw("f same");
				}
				g->SetLineColor(kBlack);
				g->SetLineWidth(2);
				g->Draw("l same");
				
				TLatex text;
				text.SetTextFont(gStyle->GetTextFont());
				text.SetTextColor(kBlack);
				text.SetTextSize(hist->GetYaxis()->GetLabelSize());
				
				Double_t x, y, z, e, angle = 0;
				
				text.SetTextAlign(22);
				x = (b->GetXMin()+b->GetXMax())/2;
				if (pad1->GetLogx()) {
					if (x > 0)  x  = TMath::Log10(x);
					else continue;
				}
				y = (b->GetYMin()+b->GetYMax())/2;
				if (pad1->GetLogy()) {
					if (y > 0)  y  = TMath::Log10(y);
					else continue;
				}
				
				text.DrawLatex(x,y,bin_name[counter].c_str());
			}
			++counter;
		}
	}
	
	hist->Draw("sameaxis"); //draw again so axes on top

	//print image
	string printformats[] = {"png","eps"};
	for(unsigned p = 0; p < 2; ++p){
		can->Print((oname+"."+printformats[p]).c_str(),printformats[p].c_str());
	}
	system(("epstopdf "+oname+".eps").c_str());
}