//custom headers
#include "KCode/KMap.h"
#include "KCode/KPlot.h"
#include "KCode/KLegend.h"
#include "KCode/KParser.h"

//ROOT headers
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>

//STL headers
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

//make plot of points, fit, band
void plotPileupAcc(string fname, vector<string> printformats={"png"}){
	//get objects from skim file
	TFile* file = TFile::Open(fname.c_str());
	TGraphAsymmErrors* g_pts = (TGraphAsymmErrors*)file->Get("pileupAccPts");
	TF1* g_fit = (TF1*)file->Get("pileupAccFit");
	TGraphErrors* g_band = (TGraphErrors*)file->Get("pileupAccBand");
	
	//formatting
	g_pts->SetLineColor(kBlack);
	g_pts->SetMarkerColor(kBlack);
	g_pts->SetMarkerStyle(20);
	g_fit->SetLineColor(kRed);
	g_fit->SetLineWidth(2);
	g_band->SetFillColor(kGray);
	
	//make some assumptions based on linear fit
	//negative slope: legend on right side
	int npts = g_band->GetN();
	KLegend::Horz legside;
	double ymin, ymax;
	if(g_fit->GetParameter(1)<0){
		legside = KLegend::right;
		ymax = g_band->GetY()[0]+g_band->GetEY()[0];
		ymin = g_band->GetY()[npts-1]-g_band->GetEY()[npts-1];
	}
	else{
		legside = KLegend::left;
		ymin = g_band->GetY()[0]-g_band->GetEY()[0];
		ymax = g_band->GetY()[npts-1]+g_band->GetEY()[npts-1];
	}

	//setup plot
	vector<string> fields;
	KParser::process(fname,'/',fields);
	string setname = fields.back().substr(5,fields.back().length()-10);
	string hname = "pileupAcc_"+setname;
	TH1F* hbase = new TH1F("hbase","",1,0,npts-1);
	hbase->GetYaxis()->SetRangeUser(ymin,ymax);
	hbase->GetXaxis()->SetTitle("number of vertices");
	hbase->GetYaxis()->SetTitle("relative acceptance");
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<string>("lumi_text","(13 TeV)");
	globalOpt->Set<double>("sizeSymb",15);
	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	localOpt->Set<bool>("logy",false);
	KPlot* plot = new KPlot(hname,localOpt,globalOpt);
	plot->Initialize(hbase);
	
	KLegend* kleg = plot->GetLegend();
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	
	//build legend
	kleg->AddHist(plot->GetHisto()); //for tick sizes
	kleg->AddEntry(g_pts,setname,"pel");
	kleg->AddEntry(g_fit,"linear fit","l");
	kleg->AddEntry(g_band,"confidence band","f");
	kleg->Build(legside,KLegend::top);
	
	//draw
	pad1->cd();
	plot->DrawHist();
	g_band->Draw("3 same");
	g_pts->Draw("p same");
	g_fit->Draw("same");
	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();
	
	//print image
	for(const auto& printformat : printformats){
		can->Print((hname+"."+printformat).c_str(),printformat.c_str());
		if(printformat=="eps") system(("epstopdf "+hname+".eps").c_str());
	}
}
