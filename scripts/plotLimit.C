#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>
#include <TGraph2D.h>
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
#include <set>
#include <map>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <utility>
#include <exception>

#include "KCode/KPlot.h"
#include "KCode/KMap.h"
#include "KCode/KMath.h"
#include "KCode/KParser.h"

using namespace std;

class Limits {
	public:
		Limits(unsigned n, double* rs, double* xs, double* v1s, double* v2s=nullptr){
			//get indices from sorting by v
			vector<unsigned> order(n);
			iota(order.begin(),order.end(),0);
			sort(order.begin(),order.end(),[&](unsigned i, unsigned j){ return v1s[i] < v1s[j] or (v2s and v1s[i]==v1s[j] and v2s[i]<v2s[j]); });
			//apply sort
			KMath::apply_permutation_in_place(n,rs,order);
			KMath::apply_permutation_in_place(n,xs,order);
			KMath::apply_permutation_in_place(n,v1s,order);
			if(v2s) KMath::apply_permutation_in_place(n,v2s,order);

			//copy data and compute derived quantities
			r.reserve(n);
			x.reserve(n);
			s.reserve(n);
			v1.reserve(n);
			if(v2s) {
				v2.reserve(n);
				l.reserve(n);
			}
			for(unsigned i = 0; i < n; ++i){
				r.push_back(rs[i]);
				x.push_back(xs[i]);
				s.push_back(rs[i]*xs[i]);
				v1.push_back(v1s[i]);
				if(v2s) {
					v2.push_back(v2s[i]);
					l.push_back(-log10(rs[i]));
				}
			}
		}
		//s = r * x, cross section limit
		//l = log10(1/r), for 2D exclusion contours
		vector<double> r, x, s, v1, v2, l;
};

Limits getLimit(TTree* limit, const string& dname, const string& cname, double q){
	const double eps = 0.01;
	stringstream ss;
	ss << "abs(quantileExpected-" << q << ")<" << eps;
	int npts = limit->Draw(dname.c_str(),(cname+ss.str()).c_str(),"goff");
	bool is2D = count(dname.begin(),dname.end(),':') >= 3;
	Limits lim(npts,limit->GetV1(),limit->GetV2(),limit->GetV3(),is2D ? limit->GetV4() : nullptr);
	return lim;
}

TGraph* getBand(TTree* limit, string dname, string cname, double q_dn, double q_up){
	const auto& lim_dn = getLimit(limit,dname,cname,q_dn);
	int npts = lim_dn.r.size();
	
	double* stmp = new double[npts*2];
	double* vtmp = new double[npts*2];
	for(int m = 0; m < npts; ++m){
		stmp[npts*2-1-m] = lim_dn.s[m];
		vtmp[npts*2-1-m] = lim_dn.v1[m];
	}

	const auto& lim_up = getLimit(limit,dname,cname,q_up);
	npts = lim_up.r.size();

	for(int m = 0; m < npts; ++m){
		stmp[m] = lim_up.s[m];
		vtmp[m] = lim_up.v1[m];
	}

	TGraph* gtmp = new TGraph(npts*2,vtmp,stmp);
	return gtmp;
}

set<double> getRange(int n, const double* arr, double& ymin, double& ymax, bool do_set=false){
	double ymin_ = TMath::MinElement(n,arr);
	if(ymin_ < ymin) ymin = ymin_;
	
	double ymax_ = TMath::MaxElement(n,arr);
	if(ymax_ > ymax) ymax = ymax_;

	set<double> vals;
	if(do_set) vals.insert(arr, arr+n);
	return vals;
}

string alphaName(double val){
	string name = "^{}#alpha_{dark}^{";
	if(val==-2) name += "peak";
	else if(val==-1) name += "high";
	else if(val==-3) name += "low";
	else throw runtime_error("Unknown alpha value: "+to_string(val));
	name += "}";
	return name;
}

//usage:
//root -l 'plotLimit.C+("svj1",{{"mZprime",-1},{"mDark",20},{"rinv",0.3},{"alpha",0.2}},{"i:nsigma[2]"})'
void plotLimit(string sname, vector<pair<string,double>> vars, vector<string> options={}){
	OptionMap* globalOpt = new OptionMap();
	for(const auto& opt : options){
		KParser::processOption(opt,globalOpt);
	}
	double varied = -1; globalOpt->Get("varied",varied);
	int nsigma = 0; globalOpt->Get("nsigma",nsigma);
	string region_text; globalOpt->Get("region_text",region_text);
	bool do_obs = globalOpt->Get("do_obs",false);
	string obs_text("Observed"); globalOpt->Get("obs_text",obs_text);
	string lumi_text("(13 TeV)"); globalOpt->Get("lumi_text",lumi_text);

	//ranges for plotting
	double ymin = 1e10, xmin = 1e10;
	double ymax = 0, xmax = -1e10;
	gStyle->SetOptStat(0);

	//extract info from hadded limit file
	string fname = "limit_"+sname+".root";
	TFile* file = KOpen(fname);
	TTree* limit = KGet<TTree>(file,"limit");

	//figure out which is varied
	stringstream scname;
	string var1, var2;
	double eps = .0001;
	for(const auto& v : vars){
		if(v.second==varied){
			if(var1.empty()) var1 = v.first;
			else var2 = v.first;
		}
		//leave a trailing && to combine with quantile requirement later
		else scname << "abs(trackedParam_" << v.first << "-" << v.second << ")<" << eps << "&&";
	}
	string cname = scname.str();

	if(var1.empty()) {
		throw runtime_error("No varied parameter specified");
	}

	//common info
	string xname, yname, zname;
	map<string,string> vdict{
		{"mZprime","^{}m_{Z'}"},
		{"mDark","^{}m_{dark}"},
		{"rinv","^{}r_{inv}"},
		{"alpha","^{}#alpha_{dark}"},
	};
	map<string,string> unitdict{
		{"mZprime"," [GeV]"},
		{"mDark"," [GeV]"},
		{"rinv",""},
		{"alpha",""},
	};
	KMap<vector<string>> labels;
	labels.Add("alpha",{alphaName(-3.),alphaName(-2.),alphaName(-1.)});
	xname = vdict[var1]+unitdict[var1];
	string dname = "limit:trackedParam_xsec:trackedParam_"+var1;

	//1D plot
	if(var2.empty()) {
		//setup plotting options
		yname = "#sigma#timesB [pb]";

		//get observed limit (w/ xsec)
		const auto& lim_obs = getLimit(limit,dname,cname,-1);
		int npts = lim_obs.r.size();
		TGraph* g_obs = new TGraph(npts,lim_obs.v1.data(),lim_obs.s.data());
		g_obs->SetMarkerColor(kBlack);
		g_obs->SetLineColor(kBlack);
		g_obs->SetMarkerStyle(20);
		g_obs->SetLineStyle(1);
		g_obs->SetLineWidth(2);
		getRange(npts,lim_obs.s.data(),ymin,ymax);

		//get cross section
		TGraph* g_xsec = new TGraph(npts,lim_obs.v1.data(),lim_obs.x.data());
		g_xsec->SetLineColor(kMagenta);
		g_xsec->SetLineStyle(1);
		g_xsec->SetLineWidth(2);
		getRange(npts,lim_obs.x.data(),ymin,ymax);
		//only get x range once
		const auto& xvals = getRange(npts,lim_obs.v1.data(),xmin,xmax,true);

		//get central value (expected)
		const auto& lim_cen = getLimit(limit,dname,cname,0.5);
		npts = lim_cen.r.size();
		TGraph* g_central = new TGraph(npts,lim_cen.v1.data(),lim_cen.s.data());
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
		//avoid huge range
		ymin = max(ymin,1e-5);
		//xmax = xmax + 100;
		//xmin = xmin - 100;

		//setup plotting options
		OptionMap* plotOpt = new OptionMap();
		plotOpt->Set<string>("lumi_text",lumi_text);
		plotOpt->Set<bool>("checkerr",false);
		plotOpt->Set<int>("npanel",1);
		//plotOpt->Set<bool>("balance_panels",true);
		plotOpt->Set<double>("ymin",ymin);
		plotOpt->Set<double>("sizeLeg",20);
		//plotOpt->Set<double>("sizeSymb",0.2);

		OptionMap* localOpt = new OptionMap();
		localOpt->Set<bool>("ratio",false);
		localOpt->Set<bool>("logy",true);
		localOpt->Set<int>("xnum",xvals.size());
		localOpt->Set<double>("xmin",xmin);
		localOpt->Set<double>("xmax",xmax);
		localOpt->Set<string>("xtitle",xname);
		localOpt->Set<string>("ytitle",yname);
		if (labels.Has(var1)){
			const auto& label = labels.Get(var1);
			if (label.size()==xvals.size()){
				localOpt->Set<bool>("xbinlabel",true);
				localOpt->Set<vector<string>>("xlabels",label);
			}
		}

		//make plot
		stringstream soname;
		soname << "plotLimit_" << sname << "_vs_" << var1;
		if(do_obs) soname << "_obs";
		string oname = soname.str();
		KParser::clean(oname);
		KPlot* plot = new KPlot(oname,localOpt,plotOpt);
		//make histo for axes
		plot->Initialize();
		auto hbase = plot->GetHisto();
		hbase->GetYaxis()->SetRangeUser(ymin,ymax);
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
		if(!region_text.empty()) kleg->AddEntry((TObject*)NULL,region_text,"");
		kleg->AddEntry(g_xsec,"Theoretical","l");
		if(do_obs) kleg->AddEntry(g_obs,obs_text,"pe");
		kleg->AddEntry(g_central,"Median expected","l");
		if(nsigma>=1) kleg->AddEntry(g_one,"68% expected","f");
		if(nsigma>=2) kleg->AddEntry(g_two,"95% expected","f");
		stringstream vname;
		for(const auto& v : vars){
			if(v.first==var1) continue;
			vname << vdict[v.first] << " = ";
			if(v.first=="alpha" and v.second<0) vname << alphaName(v.second);
			else vname << v.second;
			vname << ", ";
		}
		string svname = vname.str(); svname.pop_back(); svname.pop_back();
		kleg->AddEntry((TObject*)NULL,svname,"");
		kleg->Build(KLegend::right,KLegend::top);

		//draw blank histo for axes
		plot->DrawHist();

		//draw graphs
		if(nsigma>=2) g_two->Draw("f same");
		if(nsigma>=1) g_one->Draw("f same");
		//g_central->Draw("C same");
		g_central->Draw("L same");
		if(do_obs) g_obs->Draw("pC same");
		g_xsec->Draw("C same");

		plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
		plot->DrawText();

		//print image
		can->Print((plot->GetName()+".png").c_str(),"png");
		can->Print((plot->GetName()+".eps").c_str(),"eps");
		system(("epstopdf "+plot->GetName()+".eps && rm "+plot->GetName()+".eps").c_str());
	}
	//2D plot
	else {
		//setup plotting options
		yname = vdict[var2]+unitdict[var2];
		zname = "95% CL upper limit on #sigma#timesB [pb]";
		dname += ":trackedParam_"+var2;

		//get observed limit (w/ xsec)
		const auto& lim_obs = getLimit(limit,dname,cname,-1);

		//get central value (expected)
		const auto& lim_cen = getLimit(limit,dname,cname,0.5);

	}
}
