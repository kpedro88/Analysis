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
#include <TStyle.h>
#include <TVirtualPadPainter.h>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <utility>

#include "KCode/KPlot.h"

using namespace std;

struct Limit {
	//s = r * x
	double r, x, v, s;
};

class Limits {
	public:
		Limits(unsigned n, double* rs, double* vs, double* xs){
			vector<Limit> data;
			data.reserve(n);
			for(unsigned i = 0; i < n; ++i){
				Limit tmp;
				tmp.r = rs[i];
				tmp.x = xs[i];
				tmp.v = vs[i];
				tmp.s = tmp.r*tmp.x;
				data.push_back(tmp);
			}
			
			//sort by v
			std::sort(data.begin(),data.end(),[](const Limit& a, const Limit& b){ return a.v < b.v; });
			
			//rearrange
			r.reserve(n);
			x.reserve(n);
			v.reserve(n);
			s.reserve(n);
			for(unsigned i = 0; i < n; ++i){
				r.push_back(data[i].r);
				x.push_back(data[i].x);
				v.push_back(data[i].v);
				s.push_back(data[i].s);
			}
		}
		vector<double> r, x, v, s;
};

void multiplyXsec(unsigned n, double* rvals, double* xsecs){
	for(unsigned i = 0; i < n; ++i){
		rvals[i] = rvals[i]*xsecs[i];
	}
}

TGraph* getBand(TTree* limit, string dname, string cname, double q_dn, double q_up){
	stringstream ss_dn;
	ss_dn << "abs(quantileExpected-" << q_dn << ")<0.01";
	int npts = limit->Draw(dname.c_str(),(cname+ss_dn.str()).c_str(),"goff");
	Limits lim_dn(npts,limit->GetV1(),limit->GetV2(),limit->GetV3());
	
	double* stmp = new double[npts*2];
	double* vtmp = new double[npts*2];
	for(int m = 0; m < npts; ++m){
		stmp[npts*2-1-m] = lim_dn.s[m];
		vtmp[npts*2-1-m] = lim_dn.v[m];
	}
	
	stringstream ss_up;
	ss_up << "abs(quantileExpected-" << q_up << ")<0.01";
	npts = limit->Draw(dname.c_str(),(cname+ss_up.str()).c_str(),"goff");
	Limits lim_up(npts,limit->GetV1(),limit->GetV2(),limit->GetV3());
	
	for(int m = 0; m < npts; ++m){
		stmp[m] = lim_up.s[m];
		vtmp[m] = lim_up.v[m];
	}
	
	TGraph* gtmp = new TGraph(npts*2,vtmp,stmp);
	return gtmp;
}

void getRange(int n, double* arr, double& ymin, double& ymax){
	double ymin_ = TMath::MinElement(n,arr);
	if(ymin_ < ymin) ymin = ymin_;
	
	double ymax_ = TMath::MaxElement(n,arr);
	if(ymax_ > ymax) ymax = ymax_;
}

//usage:
//root -l 'plotLimit.C+("svj1",{{"mZprime",-1},{"mDark",20},{"rinv",0.3},{"alpha",0.2}},2)'
void plotLimit(string sname, vector<pair<string,double>> vars, int nsigma=0){
	//ranges for plotting
	double ymin = 1e10, xmin = 1e10;
	double ymax = 0, xmax = 0;
	gStyle->SetOptStat(0);
	
	//extract info from hadded limit file
	string fname = "limit_"+sname+".root";
	TFile* file = TFile::Open(fname.c_str());
	if(!file) {
		cout << "Couldn't open " << fname << endl;
		return;
	}
	TTree* limit = (TTree*)file->Get("limit");
	if(!limit) {
		cout << "Couldn't get limit tree from " << fname << endl;
		return;
	}

	//figure out which is varied
	stringstream scname;
	string var;
	double eps = .0001;
	for(const auto& v : vars){
		if(v.second==-1) var = v.first;
		//leave a trailing && to combine with quantile requirement later
		else scname << "abs(trackedParam_" << v.first << "-" << v.second << ")<" << eps << "&&";
	}
	string cname = scname.str();
	
	if(var.empty()) {
		cout << "No varied parameter specified" << endl;
		return;
	}
	
	//setup plotting options
	string dname = "limit:trackedParam_"+var+":trackedParam_xsec";
	string yname, xname;
	yname = "#sigma#timesB [pb]";
	if(var=="mZprime") xname = "m_{Z'} [GeV]";
	else if(var=="mDark") xname = "m_{d} [GeV]";
	else if(var=="rinv") xname = "r_{inv}";
	else if(var=="alpha") xname = "#alpha_{d}";
	
	//get observed limit (w/ xsec)
	int npts = limit->Draw(dname.c_str(),(cname+"abs(quantileExpected+1)<0.01").c_str(),"goff");
	Limits lim_obs(npts,limit->GetV1(),limit->GetV2(),limit->GetV3());
	TGraph* g_obs = new TGraph(npts,lim_obs.v.data(),lim_obs.s.data());
	g_obs->SetMarkerColor(kBlack);
	g_obs->SetLineColor(kBlack);
	g_obs->SetMarkerStyle(20);
	g_obs->SetLineStyle(1);
	g_obs->SetLineWidth(2);
	getRange(npts,lim_obs.s.data(),ymin,ymax);
	
	//get cross section
	TGraph* g_xsec = new TGraph(npts,lim_obs.v.data(),lim_obs.x.data());
	g_xsec->SetLineColor(kMagenta);
	g_xsec->SetLineStyle(1);
	g_xsec->SetLineWidth(2);
	getRange(npts,lim_obs.x.data(),ymin,ymax);
	//only get x range once
	getRange(npts,lim_obs.v.data(),xmin,xmax);
	
	//get central value (expected)
	npts = limit->Draw(dname.c_str(),(cname+"abs(quantileExpected-0.5)<0.01").c_str(),"goff");
	Limits lim_cen(npts,limit->GetV1(),limit->GetV2(),limit->GetV3());
	TGraph* g_central = new TGraph(npts,lim_cen.v.data(),lim_cen.s.data());
	g_central->SetLineColor(kBlue);
	g_central->SetLineStyle(2);
	g_central->SetLineWidth(2);
	getRange(npts,lim_cen.s.data(),ymin,ymax);
	
	//get bands (expected)
	TGraph* g_one = NULL;
	if(nsigma>=1){
		g_one = getBand(limit,dname,cname,0.16,0.84);
		g_one->SetFillColor(kGreen+1);
		getRange(npts*2,g_one->GetY(),ymin,ymax);
	}
	TGraph* g_two = NULL;
	if(nsigma>=2){
		g_two = getBand(limit,dname,cname,0.025,0.975);
		g_two->SetFillColor(kOrange);
		getRange(npts*2,g_two->GetY(),ymin,ymax);
	}
	
	//extend range
	ymax = ymax*10;
	ymin = ymin/10;
	//xmax = xmax + 100;
	//xmin = xmin - 100;

	//setup plotting options
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<string>("lumi_text","137 fb^{-1} (13 TeV)");
	globalOpt->Set<bool>("checkerr",false);
	globalOpt->Set<int>("npanel",1);
	//globalOpt->Set<bool>("balance_panels",true);
	globalOpt->Set<double>("ymin",ymin);
	globalOpt->Set<double>("sizeLeg",20);
	//globalOpt->Set<double>("sizeSymb",0.2);
	
	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	localOpt->Set<bool>("logy",true);
	
	//make plot
	//make histo for axes
	TH1F* hbase = new TH1F("hbase","",100,xmin,xmax);
	hbase->GetYaxis()->SetTitle(yname.c_str());
	hbase->GetXaxis()->SetTitle(xname.c_str());
	hbase->GetYaxis()->SetRangeUser(ymin,ymax);

	stringstream soname;
	soname << "plotLimit_" << sname << "_vs_" << var;
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

	//preamble of legend
	kleg->AddEntry((TObject*)NULL,"95% CL upper limits","");
	kleg->AddEntry(g_xsec,"Theoretical","l");
	//kleg->AddEntry(g_obs,"Observed","pe");
	kleg->AddEntry(g_central,"Median expected","l");
	if(nsigma>=1) kleg->AddEntry(g_one,"68% expected","f");
	if(nsigma>=2) kleg->AddEntry(g_two,"95% expected","f");
	kleg->Build(KLegend::right,KLegend::top);
	
	//draw blank histo for axes
	plot->DrawHist();
	
	//draw graphs
	if(nsigma>=2) g_two->Draw("f same");
	if(nsigma>=1) g_one->Draw("f same");
	//g_central->Draw("C same");
	g_central->Draw("L same");
	//g_obs->Draw("pC same");
	g_xsec->Draw("C same");

	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();
	
	//print image
	can->Print((plot->GetName()+".png").c_str(),"png");
}
