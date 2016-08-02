#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TMarker.h>
#include <TBox.h>
#include <TLine.h>
#include <TMath.h>
#include <TAxis.h>
#include <TVirtualPadPainter.h>

#include "Analysis/KCode/KParser.h"
#include "Analysis/KCode/KMap.h"
#include "Analysis/KCode/KPlot.h"
#include "Analysis/KCode/KLegend.h"
#include "Analysis/KCode/KStyle.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>

using namespace std;

struct Sample {
	string name;
	OptionMap* localOpt;
	KStyle* style;
	TGraph *g_central, *g_one, *g_two;
};

vector<Sample*> parseInput(string fname){
	vector<Sample*> results;
	string line;
	ifstream infile(fname.c_str());
	if(infile.is_open()){
		while(getline(infile,line)){
			//skip commented lines
			if(line[0]=='#') continue;
			//skip blank lines
			if(line.size()<2) continue;
			
			vector<string> fields;
			KNamed* tmp = KParser::processNamed(line);
			Sample* sam = new Sample;
			sam->name = tmp->first;
			sam->localOpt = tmp->second;
			sam->style = new KStyle("limit",NULL,sam->localOpt);
			sam->g_central = NULL; sam->g_one = NULL; sam->g_two = NULL;
			results.push_back(sam);
		}
	}
	else {
		cout << "Input error: could not open input file \"" << fname << "\"." << endl;
	}
	return results;
}

TGraph* getBand(TTree* limit, double q_dn, double q_up){
	stringstream ss_dn;
	ss_dn << "abs(quantileExpected-" << q_dn << ")<0.01";
	int npts = limit->Draw("limit:mh",ss_dn.str().c_str(),"goff");
	double* rtmp_dn = limit->GetV1();
	double* mtmp_dn = limit->GetV2();
	
	double* rtmp = new double[npts*2];
	double* mtmp = new double[npts*2];
	for(unsigned m = 0; m < npts; ++m){
		rtmp[npts*2-1-m] = rtmp_dn[m];
		mtmp[npts*2-1-m] = mtmp_dn[m];
	}
	
	stringstream ss_up;
	ss_up << "abs(quantileExpected-" << q_up << ")<0.01";
	npts = limit->Draw("limit:mh",ss_up.str().c_str(),"goff");
	double* rtmp_up = limit->GetV1();
	double* mtmp_up = limit->GetV2();
	
	for(unsigned m = 0; m < npts; ++m){
		rtmp[m] = rtmp_up[m];
		mtmp[m] = mtmp_up[m];
	}
	
	TGraph* gtmp = new TGraph(npts*2,mtmp,rtmp);
	return gtmp;
}

void getRange(int n, double* arr, double& ymin, double& ymax){
	double ymin_ = TMath::MinElement(n,arr);
	if(ymin_ < ymin) ymin = ymin_;
	
	double ymax_ = TMath::MaxElement(n,arr);
	if(ymax_ > ymax) ymax = ymax_;
}

double* getRatio(int n, double* numer, double* denom){
	double* ratio = new double[n];
	for(unsigned r = 0; r < n; ++r){
		ratio[r] = numer[r]/denom[r];
	}
	return ratio;
}

//usage:
//root -l 'plotLimit.C+("input/input_limit_T1bbbb_1400.txt",1400,2)'
//root -l 'plotLimit.C+("input/input_limit_T1bbbb_1000.txt",1000,2)'

void plotLimit(string input, double mMom=1400, int nsigma=0, bool logy=true){
	//list of samples and options
	vector<Sample*> samples = parseInput(input);
	if(samples.size()==0) return;
	
	//extract info from limit files
	double ymin = 1e10, xmin = 1e10;
	double ymax = 0, xmax = 0;
	vector<TGraph*> ratios;
	vector<KStyle*> ratio_styles;
	vector<Sample*> numers, denoms;
	for(unsigned s = 0; s < samples.size(); ++s){
		string fname; samples[s]->localOpt->Get("filename",fname);
		TFile* file = TFile::Open(fname.c_str());
		TTree* limit = (TTree*)file->Get("limit");
		
		//get central value
		int npts = limit->Draw("limit:mh","abs(quantileExpected-0.5)<0.01","goff");
		double* rtmp = limit->GetV1();
		double* mtmp = limit->GetV2();
		samples[s]->g_central = new TGraph(npts,mtmp,rtmp);
		samples[s]->style->Format(samples[s]->g_central);
		getRange(npts,rtmp,ymin,ymax);
		//only get x range once
		getRange(npts,mtmp,xmin,xmax);
		
		if(nsigma>=1){
			samples[s]->g_one = getBand(limit,0.16,0.84);
			samples[s]->style->Format(samples[s]->g_one);
			if(nsigma<2) samples[s]->g_one->SetFillStyle(3013);
			else samples[s]->g_one->SetFillStyle(3004);
			getRange(npts*2,samples[s]->g_one->GetY(),ymin,ymax);
		}
		if(nsigma>=2){
			samples[s]->g_two = getBand(limit,0.025,0.975);
			samples[s]->style->Format(samples[s]->g_two);
			samples[s]->g_two->SetFillStyle(3005);
			getRange(npts*2,samples[s]->g_two->GetY(),ymin,ymax);
		}
		
		//todo: include cross-section multiplication
		
		//check ratio options
		if(samples[s]->localOpt->Get("denom",false)) denoms.push_back(samples[s]);
		if(samples[s]->localOpt->Get("numer",false)) numers.push_back(samples[s]);
	}
	
	//make ratios
	bool ratio = true;
	string rationame = "";
	double rmin = 0.75, rmax = 1.1;
	if(numers.size()==1 && denoms.size()==1){
		TGraph* gtmp = new TGraph(numers[0]->g_central->GetN(),numers[0]->g_central->GetX(),getRatio(numers[0]->g_central->GetN(),numers[0]->g_central->GetY(),denoms[0]->g_central->GetY()));
		KStyle* stmp = new KStyle(*(numers[0]->style));
		ratios.push_back(gtmp);
		ratio_styles.push_back(stmp);
		stmp->Format(gtmp);
		rationame = numers[0]->name+"/"+denoms[0]->name;
		getRange(gtmp->GetN(),gtmp->GetY(),rmin,rmax);
	}
	else if(numers.size()==1 && denoms.size()>1){
		for(unsigned r = 0; r < denoms.size(); r++){
			TGraph* gtmp = new TGraph(numers[0]->g_central->GetN(),numers[0]->g_central->GetX(),getRatio(numers[0]->g_central->GetN(),numers[0]->g_central->GetY(),denoms[r]->g_central->GetY()));
			KStyle* stmp = new KStyle(*(denoms[r]->style));
			ratios.push_back(gtmp);
			ratio_styles.push_back(stmp);
			stmp->Format(gtmp);
			getRange(gtmp->GetN(),gtmp->GetY(),rmin,rmax);
		}
		rationame = numers[0]->name+"/new";
	}
	else if(numers.size()>1 && denoms.size()==1){
		for(unsigned r = 0; r < numers.size(); r++){
			TGraph* gtmp = new TGraph(numers[r]->g_central->GetN(),numers[r]->g_central->GetX(),getRatio(numers[r]->g_central->GetN(),numers[r]->g_central->GetY(),denoms[0]->g_central->GetY()));
			KStyle* stmp = new KStyle(*(numers[r]->style));
			ratios.push_back(gtmp);
			ratio_styles.push_back(stmp);
			stmp->Format(gtmp);
			getRange(gtmp->GetN(),gtmp->GetY(),rmin,rmax);
		}
		rationame = "new/"+denoms[0]->name;
	}
	else ratio = false;
	
	//extend range
	//double yrange = ymax - ymin;
	//ymax = ymax + yrange*0.1;
	//ymin = ymin - yrange*0.1;
	//double xrange = xmax - xmin;
	//xmax = xmax + xrange*0.1;
	//xmin = xmin - xrange*0.1;
	//cout << "x: (" << xmin << ", " << xmax << ")" << endl;
	//cout << "y: (" << ymin << ", " << ymax << ")" << endl;
	
	//setup plotting options
	vector<string> extra_text;
	extra_text.push_back("#tilde{g} #rightarrow b #bar{b} #tilde{#chi}_{1}^{0}");
	stringstream st;
	st << "m_{#tilde{g}} = " << mMom;
	extra_text.push_back(st.str());
	//vector<int> extra_text_panels;
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<string>("lumi_text","2.3 fb^{-1} (13 TeV)");
	globalOpt->Set<bool>("checkerr",false);
	globalOpt->Set<int>("npanel",2);
	globalOpt->Set<bool>("balance_panels",true);
	globalOpt->Set<double>("ymin",ymin);
	globalOpt->Set<vector<string> >("extra_text",extra_text);
	//globalOpt->Set<vector<int> >("extra_text_panels",extra_text_panels);
	//globalOpt->Set<double>("sizeLeg",21.5);
	globalOpt->Set<double>("sizeSymb",0.2);
	globalOpt->Set<Color_t>("ratiolinecolor",kBlack);
	globalOpt->Set<int>("ratiolinewidth",1);
	globalOpt->Set<double>("ratiomin",rmin);
	globalOpt->Set<double>("ratiomax",rmax);
	globalOpt->Set<string>("rationame",rationame);
	
	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",ratio);
	localOpt->Set<bool>("logy",logy);
	
	//make plot
	TH1F* hbase = new TH1F("hbase","",100,xmin,xmax);
	hbase->GetYaxis()->SetTitle("Expected Limit");
	hbase->GetXaxis()->SetTitle("m_{#tilde{#chi}_{1}^{0}} [GeV]");
	hbase->GetYaxis()->SetRangeUser(ymin,ymax);
	stringstream soname;
	soname << "plotLimit_mGl-" << mMom;
	if(logy) soname << "_logy";
	string oname = soname.str();
	KParser::clean(oname);
	KPlot* plot = new KPlot(oname,localOpt,globalOpt);
	plot->Initialize(hbase);
	KLegend* kleg = plot->GetLegend();
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();
	pad1->SetGridx();
	pad1->SetGridy();
	
	//build legend
	kleg->AddHist(plot->GetHisto()); //for tick sizes
	if(nsigma>=1){
		TBox* b = new TBox();
		b->SetFillColor(kBlack);
		b->SetLineColor(kBlack);
		b->SetFillStyle(3013);
		kleg->AddEntry(b,"#pm 1 #sigma","f");
	}
	if(nsigma>=2){
		TBox* b = new TBox();
		b->SetFillColor(kBlack);
		b->SetLineColor(kBlack);
		b->SetFillStyle(3005);
		kleg->AddEntry(b,"#pm 2 #sigma","f");
	}
	for(unsigned s = 0; s < samples.size(); ++s){
		string legname; samples[s]->localOpt->Get("legname",legname);
		string legopt; samples[s]->localOpt->Get("legopt",legopt);
		kleg->AddEntry(samples[s]->g_central,legname,legopt);
	}
	
	kleg->Build(KLegend::left,KLegend::top);
	
	//draw blank histo for axes
	plot->DrawHist();
	
	//draw graphs
	for(unsigned s = 0; s < samples.size(); ++s){
		if(nsigma>=2) samples[s]->g_two->Draw("f same");
		if(nsigma>=1) samples[s]->g_one->Draw("f same");
		samples[s]->g_central->Draw(samples[s]->style->GetDrawOpt("same").c_str());
	}
	//draw line
	TLine* line = new TLine(xmin,1,xmax,1);
	line->SetLineColor(kBlack);
	line->SetLineWidth(3);
	line->SetLineStyle(2);
	line->Draw("same");

	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();
	
	//draw ratio if enabled
	if(ratio){
		TPad* pad2 = plot->GetPad2();
		pad2->cd();
		
		plot->DrawRatio();
		for(unsigned r = 0; r < ratios.size(); ++r){
			ratios[r]->Draw(ratio_styles[r]->GetDrawOpt("same").c_str());
		}
		plot->DrawLine();
	}
	
	//print image
	string printformats[] = {"png","eps"};
	for(unsigned p = 0; p < 1; ++p){
		can->Print((oname+"."+printformats[p]).c_str(),printformats[p].c_str());
		if(printformats[p]=="eps") system(("epstopdf "+oname+".eps").c_str());
	}
}