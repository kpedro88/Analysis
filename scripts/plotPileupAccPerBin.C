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
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TGraphErrors.h>
#include <TLine.h>

//STL headers
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <numeric>

using namespace std;

//make plot of points, fit, band
void plotPileupAcc(string fname, string type, TH1* hist, double val, double err, vector<string> printformats){
	//cout << type << " = " << val << " +/- " << err << endl;
	
	//find average of hist
	TF1* g_fit = new TF1((type+"_avg").c_str(),"pol0",hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
	TFitResultPtr fptr = hist->Fit(g_fit,"NRQS");
	
	//make confidence band
	//TGraphErrors* g_band = new TGraphErrors(hist->GetNbinsX());
	//for(int i = 1; i <= hist->GetNbinsX(); ++i){
	//	g_band->SetPoint(i-1,i,g_fit->GetParameter(0));
	//}
	//fptr->GetConfidenceIntervals(g_band->GetN(), 1, 1, g_band->GetX(), g_band->GetEY(), 0.68, false);
	
	//make overall line
	TLine* line = new TLine(hist->GetXaxis()->GetXmin(),val,hist->GetXaxis()->GetXmax(),val);
	
	//make confidence band
	//vector<double> xband(hist->GetNbinsX());
	//iota(xband.begin(),xband.end(),1);
	//vector<double> yband(hist->GetNbinsX(),val);
	//vector<double> yeband(hist->GetNbinsX(),err);
	//TGraphErrors* g_band = new TGraphErrors(xband.size(),xband.data(),yband.data(),NULL,yeband.data());
	
	//formatting
	hist->SetLineColor(kBlack);
	hist->SetMarkerColor(kBlack);
	hist->SetMarkerStyle(20);
	g_fit->SetLineColor(kRed);
	g_fit->SetLineWidth(2);
	//g_band->SetFillColor(kGray);
	line->SetLineColor(kBlue);
	line->SetLineWidth(2);
	line->SetLineStyle(2);

	//setup plot
	vector<string> fields;
	KParser::process(fname,'/',fields);
	string setname = fields.back().substr(5,fields.back().length()-10);
	string hname = type+"_"+setname;
	TH1F* hbase = (TH1F*)hist->Clone("hbase");
	hbase->Reset();
	hbase->GetXaxis()->SetTitle("Search region bin number");
	hbase->GetYaxis()->SetTitle(type.c_str());
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<string>("lumi_text","(13 TeV)");
	globalOpt->Set<double>("sizeSymb",0.15);
	globalOpt->Set<double>("NdivX",518);
	globalOpt->Set<double>("canvasW",1200);
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
	kleg->AddEntry(hist,setname,"pel");
	kleg->AddEntry(line,"overall","l");
	//kleg->AddEntry(g_band,"confidence band","f");
	kleg->AddEntry(g_fit,"average","l");
	//kleg->AddEntry(g_band,"confidence band","f");
	kleg->SetManualYmin(hist->GetMinimum());
	kleg->Build();
	
	//to blank out zero line
	TLine* zero = new TLine(hist->GetXaxis()->GetXmin(),0,hist->GetXaxis()->GetXmax(),0);
	zero->SetLineColor(0);
	
	//draw
	pad1->cd();
	plot->DrawHist();
	if(hist->GetMinimum()<0) zero->Draw("same");
	//g_band->Draw("3 same");
	hist->Draw("pe same");
	g_fit->Draw("same");
	line->Draw("same");
	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();
	
	//print image
	for(const auto& printformat : printformats){
		can->Print(("plots-putest/"+hname+"."+printformat).c_str(),printformat.c_str());
		if(printformat=="eps") system(("epstopdf "+hname+".eps").c_str());
	}
}

void plotPileupAccPerBin(string fname, vector<string> printformats={"png"}){
	//get objects from skim file
	TFile* file = TFile::Open(fname.c_str());
	TF1* g_fit = (TF1*)file->Get("pileupAccFit");
	TH1* h_slope = (TH1*)file->Get("pu_slope");
	TH1* h_intercept = (TH1*)file->Get("pu_intercept");

	plotPileupAcc(fname,"slope",h_slope,g_fit->GetParameter(1),g_fit->GetParError(1),printformats);
	plotPileupAcc(fname,"intercept",h_intercept,g_fit->GetParameter(0),g_fit->GetParError(0),printformats);
}