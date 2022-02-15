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
#include <Math/Interpolator.h>

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
#include "KCode/KLegend.h"
#include "KCode/KMap.h"
#include "KCode/KMath.h"
#include "KCode/KParser.h"

using namespace std;

//get around hardcoded limits on nbins for generated histogram
class KGraph2D : public TGraph2D {
	public:
		void SetNpx(Int_t npx){
			fNpx = npx;
			if (fHistogram) {
				delete fHistogram;
				fHistogram = nullptr;
				fDelaunay = nullptr;
			}
		}
		void SetNpy(Int_t npy){
			fNpy = npy;
			if (fHistogram) {
				delete fHistogram;
				fHistogram = nullptr;
				fDelaunay = nullptr;
			}
		}
	};

class Limits {
	public:
		Limits() {}
		Limits(int itype, bool multxs, unsigned n, double* rs, double* xs, double* v1s, double* v2s=nullptr){
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
				x.push_back(xs[i]);
				if(multxs) {
					r.push_back(rs[i]);
					s.push_back(rs[i]*xs[i]);
				}
				else {
					r.push_back(rs[i]/xs[i]); //so findExclusionCurve will still work
					s.push_back(rs[i]);
				}
				S.push_back(log10(s.back()));
				v1.push_back(v1s[i]);
				if(v2s) {
					v2.push_back(v2s[i]);
					l.push_back(-log10(r.back()));
				}
				else l.push_back(log10(x.back()));
			}

			interp(itype);
		}
		bool empty() const { return r.empty(); }
		void interp(int itype=0, int npts=100) {
			if(itype==0 or !v2.empty()) return; //only for 1D
			bool logr = itype==1;
			vector<double> vi, si, xi;
			ROOT::Math::Interpolator interp(v1,logr ? S : s,ROOT::Math::Interpolation::kLINEAR);
			ROOT::Math::Interpolator interpx(v1,logr ? l : x,ROOT::Math::Interpolation::kLINEAR);
			for(unsigned i = 0; i < v1.size()-1; ++i){
				double delta = (v1[i+1] - v1[i])/(npts);
				for(unsigned j = 0; j < npts+1; ++j){
					vi.push_back(j==npts ? v1[i+1] : v1[i] + j*delta);
					double sval = interp.Eval(vi.back());
					si.push_back(logr ? pow(10,sval) : sval);
					double xval = interpx.Eval(vi.back());
					xi.push_back(logr ? pow(10,xval) : xval);
				}
			}
			v1_orig = v1;
			s_orig = s;
			x_orig = x;
			v1 = vi;
			s = si;
			x = xi;
		}
		//s = r * x, cross section limit
		//l = log10(1/r), for 2D exclusion contours; OR log10(x), for 1D interpolation
		//S = log10(s), for 2D exclusion histogram & 1D interpolation
		vector<double> r, x, s, v1, v2, l, S;
		vector<double> x_orig, s_orig, v1_orig;
};

Limits getLimit(TTree* limit, const string& dname, const string& cname, double q, int itype, bool multxs){
	const double eps = 0.01;
	stringstream ss;
	ss << "abs(quantileExpected-" << q << ")<" << eps;
	int npts = limit->Draw(dname.c_str(),(cname+ss.str()).c_str(),"goff");
	bool is2D = count(dname.begin(),dname.end(),':') >= 3;
	Limits lim(itype,multxs,npts,limit->GetV1(),limit->GetV2(),limit->GetV3(),is2D ? limit->GetV4() : nullptr);
	return lim;
}

TGraph* getBand(TTree* limit, string dname, string cname, double q_dn, double q_up, int itype, bool multxs){
	const auto& lim_dn = getLimit(limit,dname,cname,q_dn,itype,multxs);
	int npts_dn = lim_dn.s.size();

	const auto& lim_up = getLimit(limit,dname,cname,q_up,itype,multxs);
	int npts_up = lim_up.s.size();

	int npts = npts_dn+npts_up;
	double* stmp = new double[npts];
	double* vtmp = new double[npts];
	for(int m = 0; m < npts_dn; ++m){
		stmp[npts-1-m] = lim_dn.s[m];
		vtmp[npts-1-m] = lim_dn.v1[m];
	}

	for(int m = 0; m < npts_up; ++m){
		stmp[m] = lim_up.s[m];
		vtmp[m] = lim_up.v1[m];
	}

	TGraph* gtmp = new TGraph(npts,vtmp,stmp);
	return gtmp;
}

//based on https://gitlab.cern.ch/eno/emerginjetsPlotting/-/blob/master/emjet_exclusion_plots.py
vector<TGraph*> findExclusionCurve(Limits& lim){
	vector<TGraph*> results;
	//no points: nothing to do
	if(lim.v1.empty()) return results;

	//contours can only be obtained by drawing: put into batch mode temporarily
	bool isBatch = gROOT->IsBatch();
	gROOT->SetBatch(kTRUE);
	auto graph = new TGraph2D(lim.v1.size(),lim.v1.data(),lim.v2.data(),lim.l.data());
	auto hist = graph->GetHistogram();

	//z axis is log(1/r), so anything > 0 is excluded
	//use 1e-10 as threshold to avoid creating contour around empty parts of graph
	double contour_values[1] = {1e-10};
	hist->SetContour(1, contour_values);

	//draw to get contours
	auto can = new TCanvas();
	hist->Draw("cont list");
	can->Update();
	auto contours = (TObjArray*)(gROOT->GetListOfSpecials()->FindObject("contours"));
	auto contour_list = (TList*)(contours->At(0));

	//each contour level may have multiple graphs (in case of disconnected regions)
	results.reserve(contour_list->GetSize());
	auto curve = (TGraph*)(contour_list->First());
	for(unsigned j = 0; j < contour_list->GetSize(); ++j){
		results.push_back((TGraph*)(curve->Clone()));
		curve = (TGraph*)(contour_list->After(curve));
	}

	gROOT->SetBatch(isBatch);
	delete can;
	return results;
}

void styleGraphs(vector<TGraph*>& graphs, Color_t linecolor, int linestyle, int linewidth=3){
	//because graphs are used for legends, the vector should have at least one entry
	if(graphs.empty()) graphs.push_back(new TGraph());
	for(auto graph : graphs){
		graph->SetLineColor(linecolor);
		graph->SetLineStyle(linestyle);
		graph->SetLineWidth(linewidth);
	}
}

void writeContour(const std::string& cname, const vector<TGraph*>& v){
	for(unsigned j = 0; j < v.size(); ++j){
		stringstream ss;
		ss << "contour_" << cname << "_" << j;
		v[j]->Write(ss.str().c_str());
	}
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

string erase(string input, const vector<string>& patterns){
	for(const auto& pattern: patterns){
		size_t pos = 0;
		do {
			pos = input.find(pattern);
			if(pos!=string::npos) input.erase(pos,pattern.size());
		} while (pos!=string::npos);
	}
	return input;
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
	bool logy = globalOpt->Get("logy",true);
	bool do_obs = globalOpt->Get("do_obs",false);
	bool show_exp = globalOpt->Get("show_exp",false);
	bool interp = globalOpt->Get("interp",false);
	bool multxs = globalOpt->Get("multxs",true);
	vector<string> extra_text; bool set_extra_text = globalOpt->Get("extra_text",extra_text);
	string obs_text("Observed"); globalOpt->Get("obs_text",obs_text);
	string the_text("Theoretical"); globalOpt->Get("the_text",the_text);
	string med_text("Median expected"); globalOpt->Get("med_text",med_text);
	string one_text("68% expected"); globalOpt->Get("one_text",one_text);
	string two_text("95% expected"); globalOpt->Get("two_text",two_text);
	string lumi_text("(13 TeV)"); globalOpt->Get("lumi_text",lumi_text);
	string prelim_text; bool set_prelim = globalOpt->Get("prelim_text",prelim_text);
	string dir; globalOpt->Get("dir",dir);
	string printsuffix; globalOpt->Get("printsuffix",printsuffix);
	string limitqty = "limit"; globalOpt->Get("limitqty",limitqty);
	string xsecqty = "xsec"; globalOpt->Get("xsecqty",xsecqty);
	bool acceff = globalOpt->Get("acceff",false);
	bool interp_log = globalOpt->Get("interp_log",!acceff);
	bool logz = globalOpt->Get("logz",!acceff);
	int itype = interp ? acceff ? 2 : 1 : 0;
	string fpre(acceff ? "sigAccEff" : "limit"); globalOpt->Get("fpre",fpre);
	string plotpre(acceff ? "plotAccEff" : "plotLimit"); globalOpt->Get("plotpre",plotpre);
	string pname(acceff ? "Signal efficiency #times acceptance" : ""); globalOpt->Get("pname",pname);
	double sizeP = 0; globalOpt->Get("sizeP",sizeP);
	double sizePprelim = 0; globalOpt->Get("sizePprelim",sizePprelim);
	double sizePlumi = 0; globalOpt->Get("sizePlumi",sizePlumi);
	double yoff = 0; bool use_yoff = globalOpt->Get("yoff",yoff);
	double hscaleLeg = 0; globalOpt->Get("hscaleLeg",hscaleLeg);
	double ushiftLeg = 0; globalOpt->Get("ushiftLeg",ushiftLeg);
	double vshiftLeg = 0; globalOpt->Get("vshiftLeg",vshiftLeg);

	vector<string> base_extra_text{"95% CL upper limits"};
	if(acceff) base_extra_text = {};
	if(!set_extra_text) extra_text = base_extra_text;

	//ranges for plotting
	double ymin = 1e10, xmin = 1e10;
	double ymax = 0, xmax = -1e10;
	gStyle->SetOptStat(0);

	//extract info from hadded limit file
	string fname = fpre+"_"+sname+".root";
	if(!dir.empty()) fname = dir+"/"+fname;
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
		{"mZprime/1000","^{}m_{Z'}"},
		{"mDark","^{}m_{dark}"},
		{"rinv","^{}r_{inv}"},
		{"alpha","^{}#alpha_{dark}"},
		{"g_q","^{}g_{q}"},
		{"g_dark","^{}g_{#chi}"},
	};
	map<string,string> unitdict{
		{"mZprime"," [GeV]"},
		{"mZprime/1000"," [TeV]"},
		{"mDark"," [GeV]"},
		{"rinv",""},
		{"alpha",""},
		{"g_q",""},
		{"g_dark",""},
	};
	KMap<vector<string>> labels;
	labels.Add("alpha",{alphaName(-3.),alphaName(-2.),alphaName(-1.)});
	xname = vdict[var1]+unitdict[var1];
	string dname = limitqty+":";
	if (xsecqty.empty()) dname += "1";
	else dname += "trackedParam_"+xsecqty;
	dname += ":trackedParam_"+var1;

	//1D plot
	if(var2.empty()) {
		//setup plotting options
		yname = pname.empty() ? "#sigma B [pb]" : pname;

		TGraph *g_obs = nullptr, *g_obs_pt = nullptr, *g_xsec = nullptr;
		int npts = 0;

		if(!acceff){
			//get observed limit (w/ xsec)
			auto lim_obs = getLimit(limit,dname,cname,-1,itype,multxs);
			npts = lim_obs.s.size();
			if(npts>0){
				g_obs = new TGraph(npts,lim_obs.v1.data(),lim_obs.s.data());
				g_obs->SetMarkerColor(kBlack);
				g_obs->SetLineColor(kBlack);
				g_obs->SetMarkerStyle(20);
				g_obs->SetLineStyle(1);
				g_obs->SetLineWidth(2);
				getRange(npts,lim_obs.s.data(),ymin,ymax);

				if(interp){
					g_obs_pt = new TGraph(lim_obs.s_orig.size(),lim_obs.v1_orig.data(),lim_obs.s_orig.data());
					g_obs_pt->SetMarkerColor(kBlack);
					g_obs_pt->SetLineColor(kBlack);
					g_obs_pt->SetMarkerStyle(20);
					g_obs_pt->SetLineStyle(1);
					g_obs_pt->SetLineWidth(2);
				}
			}
			else if(do_obs){
				throw runtime_error("Did not find any observed limit values");
			}
		}

		//get central value (expected)
		const auto& lim_cen = getLimit(limit,dname,cname,0.5,itype,multxs);
		npts = lim_cen.s.size();
		TGraph* g_central = new TGraph(npts,lim_cen.v1.data(),lim_cen.s.data());
		g_central->SetLineColor(kBlue);
		g_central->SetLineStyle(2);
		g_central->SetLineWidth(2);
		getRange(npts,lim_cen.s.data(),ymin,ymax);
		//only get x range once
		const auto& xvals = getRange(npts,lim_cen.v1.data(),xmin,xmax,true);
		xmax += eps;

		if(!acceff){
			//get cross section
			g_xsec = new TGraph(npts,lim_cen.v1.data(),lim_cen.x.data());
			g_xsec->SetLineColor(kMagenta);
			g_xsec->SetLineStyle(1);
			g_xsec->SetLineWidth(2);
			getRange(npts,lim_cen.x.data(),ymin,ymax);
		}

		//get bands (expected)
		TGraph* g_one = NULL;
		if(nsigma>=1){
			g_one = getBand(limit,dname,cname,0.16,0.84,itype,multxs);
			g_one->SetFillColor(kGreen+1);
			getRange(npts*2,g_one->GetY(),ymin,ymax);
		}
		TGraph* g_two = NULL;
		if(nsigma>=2){
			g_two = getBand(limit,dname,cname,0.025,0.975,itype,multxs);
			g_two->SetFillColor(kOrange);
			getRange(npts*2,g_two->GetY(),ymin,ymax);
		}

		globalOpt->Get("ymin",ymin);
		globalOpt->Get("ymax",ymax);
		//extend range
		double yfactor = 10; globalOpt->Get("yfactor",yfactor);
		ymax = ymax*yfactor;
		ymin = ymin/yfactor;
		//avoid huge range
		ymin = max(ymin,1e-5);

		//setup plotting options
		OptionMap* plotOpt = new OptionMap();
		plotOpt->Set<vector<string>>("extra_text",extra_text);
		plotOpt->Set<string>("lumi_text",lumi_text);
		if(set_prelim) plotOpt->Set<string>("prelim_text",prelim_text);
		plotOpt->Set<bool>("checkerr",false);
		plotOpt->Set<int>("npanel",1);
		plotOpt->Set<double>("ymin",ymin);
		plotOpt->Set<double>("sizeLeg",20);
		plotOpt->Set<bool>("auto_g",globalOpt->Get("auto_g",true));
		plotOpt->Set<bool>("debug_legend",globalOpt->Get("debug_legend",false));
		if(sizeP>0) plotOpt->Set<double>("sizeP",sizeP);
		if(sizePprelim>0) plotOpt->Set<double>("sizePprelim",sizePprelim);
		if(sizePlumi>0) plotOpt->Set<double>("sizePlumi",sizePlumi);
		if(hscaleLeg!=0) plotOpt->Set<double>("hscaleLeg",hscaleLeg);
		if(ushiftLeg!=0) plotOpt->Set<double>("ushiftLeg",ushiftLeg);
		if(vshiftLeg!=0) plotOpt->Set<double>("vshiftLeg",vshiftLeg);

		OptionMap* localOpt = new OptionMap();
		localOpt->Set<bool>("ratio",false);
		localOpt->Set<bool>("logy",logy);
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
		soname << plotpre << "_" << sname << "_vs_" << var1;
		if(do_obs) soname << "_obs";
		if(!printsuffix.empty()) soname << "_" << printsuffix;
		string oname = soname.str();
		KParser::clean(oname);
		if(!dir.empty()) oname = dir+"/"+oname;
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

		//assemble legend
		kleg->AddHist(plot->GetHisto()); //for tick sizes

		//preamble of legend
		if(!region_text.empty()) kleg->AddEntry((TObject*)NULL,region_text,"");
		if(g_xsec) kleg->AddEntry(g_xsec,the_text,"l");
		if(do_obs and g_obs) kleg->AddEntry(g_obs,obs_text,"pe");
		if(!acceff) kleg->AddEntry(g_central,med_text,"l");
		if(nsigma>=1) kleg->AddEntry(g_one,one_text,"f");
		if(nsigma>=2) kleg->AddEntry(g_two,two_text,"f");
		stringstream vname;
		for(const auto& v : vars){
			if(v.first==var1) continue;
			vname << vdict[v.first] << " = ";
			if(v.first=="alpha" and v.second<0) vname << alphaName(v.second);
			else vname << v.second;
			vname << erase(unitdict[v.first],{"[","]"});
			vname << ", ";
		}
		string svname = vname.str();
		if(!svname.empty()){
			svname.pop_back(); svname.pop_back();
			kleg->AddEntry((TObject*)NULL,svname,"");
		}

		//draw blank histo for axes
		if(use_yoff) hbase->GetYaxis()->SetTitleOffset(yoff);
		plot->DrawHist();

		//build legend (after drawing axes)
		kleg->Build(KLegend::right,KLegend::top);
		//extra style options
		auto leg = kleg->GetLegend();
		leg->SetFillStyle(1001);
		leg->SetFillColorAlpha(0,0.6);

		//draw graphs
		if(nsigma>=2) g_two->Draw("f same");
		if(nsigma>=1) g_one->Draw("f same");
		g_central->Draw("L same");
		if(interp){
			if(do_obs){
				if(g_obs) g_obs->Draw("L same");
				if(g_obs_pt) g_obs_pt->Draw("p same");
			}
			if(g_xsec) g_xsec->Draw("L same");
		}
		else{
			if(do_obs and g_obs) g_obs->Draw("pC same");
			if(g_xsec) g_xsec->Draw("C same");
		}

		plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
		plot->DrawText();

		//print image
		//save directly to pdf: saving to eps and then converting to pdf loses transparent legend behavior
		can->Print((plot->GetName()+".pdf").c_str(),"pdf");
		//convert from pdf to png (better display of dashed lines)
		system(("convert -trim -density 300 "+plot->GetName()+".pdf "+plot->GetName()+".png").c_str());

		//save objects
		auto outfile = TFile::Open((plot->GetName()+".root").c_str(),"RECREATE");
		outfile->cd();
		hbase->Write();
		if(nsigma>=2) g_two->Write("g_two");
		if(nsigma>=1) g_one->Write("g_one");
		g_central->Write("g_central");
		if(do_obs and g_obs) g_obs->Write("g_obs");
		if(g_xsec) g_xsec->Write("g_xsec");
	}
	//2D plot
	else {
		//setup plotting options
		ymax = -1e10;
		yname = vdict[var2]+unitdict[var2];
		zname = pname.empty() ? string("95% CL ")+(do_obs and !show_exp ? "obs." : "exp.")+" upper limit on #sigma B [pb]" : pname;
		dname += ":trackedParam_"+var2;

		Limits lim_obs, lim_cen;
		vector<TGraph*> contours_obs, contours_cen;
		set<double> xvals, yvals;

		//only get x,y ranges once
		lim_cen = getLimit(limit,dname,cname,0.5,itype,multxs);
		int npts = lim_cen.r.size();
		xvals = getRange(npts,lim_cen.v1.data(),xmin,xmax,true);
		yvals = getRange(npts,lim_cen.v2.data(),ymin,ymax,true);

		if(!acceff){
			if(do_obs){
				//get observed limit (w/ xsec) & exclusion contour
				lim_obs = getLimit(limit,dname,cname,-1,itype,multxs);
				contours_obs = findExclusionCurve(lim_obs);
				styleGraphs(contours_obs,kBlack,1);
			}

			//get central value (expected) & exclusion contour
			contours_cen = findExclusionCurve(lim_cen);
			styleGraphs(contours_cen,kRed,1);
		}

		//get uncertainty bands and contours
		vector<Limits> lim_sigmas;
		if(nsigma>=1){
			lim_sigmas.push_back(getLimit(limit,dname,cname,0.16,itype,multxs));
			lim_sigmas.push_back(getLimit(limit,dname,cname,0.84,itype,multxs));
		}
		if(nsigma>=2){
			lim_sigmas.push_back(getLimit(limit,dname,cname,0.025,itype,multxs));
			lim_sigmas.push_back(getLimit(limit,dname,cname,0.975,itype,multxs));
		}
		vector<vector<TGraph*>> contours_sigmas;
		for(unsigned i = 0; i < lim_sigmas.size(); ++i){
			contours_sigmas.push_back(findExclusionCurve(lim_sigmas[i]));
			styleGraphs(contours_sigmas.back(), kRed, i>=2 ? 2 : 7);
		}

		//get histogram to plot
		TGraph2D* gtmp;
		if(!interp_log) gtmp = new TGraph2D(lim_cen.v1.size(),lim_cen.v1.data(),lim_cen.v2.data(),lim_cen.s.data()); //acceff doesn't use log
		else {
			if(do_obs and !show_exp) gtmp = new TGraph2D(lim_obs.v1.size(),lim_obs.v1.data(),lim_obs.v2.data(),lim_obs.S.data());
			else gtmp = new TGraph2D(lim_cen.v1.size(),lim_cen.v1.data(),lim_cen.v2.data(),lim_cen.S.data());
		}
		//control number of bins for labeled axes
		if(labels.Has(var1)) ((KGraph2D*)gtmp)->SetNpx(xvals.size());
		if(labels.Has(var2)) ((KGraph2D*)gtmp)->SetNpy(yvals.size());
		auto h2d = (TH2F*)(gtmp->GetHistogram()->Clone());
		//convert back to normal scale
		//bins with zero content and zero error correspond to empty parts of graph: set to very small value
		double empty = !interp_log ? -1 : 1e-100;
		double zmin = !interp_log ? 0 : 1e10;
		for(unsigned i = 0; i <= h2d->GetNbinsX()+1; ++i){
			for(unsigned j = 0; j <= h2d->GetNbinsY()+1; ++j){
				if(h2d->GetBinContent(i,j)==0. and h2d->GetBinError(i,j)==0.) h2d->SetBinContent(i,j,empty);
				else if(interp_log){ //acceff doesn't use log
					double new_content = pow(10,h2d->GetBinContent(i,j));
					h2d->SetBinContent(i,j,new_content);
					if(new_content<zmin) zmin = new_content;
				}
			}
		}
		//take z axis max
		double zmax = h2d->GetMaximum();
		//set range to avoid filling empty parts
		h2d->GetZaxis()->SetRangeUser(zmin,zmax);
		h2d->GetZaxis()->SetTitle(zname.c_str());

		//setup plotting options
		OptionMap* plotOpt = new OptionMap();
		plotOpt->Set<vector<string>>("extra_text",extra_text);
		plotOpt->Set<string>("lumi_text",lumi_text);
		if(set_prelim) plotOpt->Set<string>("prelim_text",prelim_text);
		plotOpt->Set<bool>("checkerr",false);
		plotOpt->Set<int>("npanel",1);
		plotOpt->Set<double>("sizeLeg",20);
		if(acceff) plotOpt->Set<double>("sizeSymb",0);
		plotOpt->Set<int>("qdefault",1);
		if(sizeP>0) plotOpt->Set<double>("sizeP",sizeP);
		if(sizePprelim>0) plotOpt->Set<double>("sizePprelim",sizePprelim);
		if(sizePlumi>0) plotOpt->Set<double>("sizePlumi",sizePlumi);
		if(hscaleLeg!=0) plotOpt->Set<double>("hscaleLeg",hscaleLeg);
		if(ushiftLeg!=0) plotOpt->Set<double>("ushiftLeg",ushiftLeg);
		if(vshiftLeg!=0) plotOpt->Set<double>("vshiftLeg",vshiftLeg);

		OptionMap* localOpt = new OptionMap();
		localOpt->Set<bool>("ratio",false);
		localOpt->Set<bool>("logz",logz);
		localOpt->Set<int>("xnum",xvals.size());
		localOpt->Set<double>("xmin",xmin);
		localOpt->Set<double>("xmax",xmax);
		localOpt->Set<string>("xtitle",xname);
		localOpt->Set<int>("ynum",yvals.size());
		localOpt->Set<double>("ymin",ymin);
		localOpt->Set<double>("ymax",ymax);
		localOpt->Set<string>("ytitle",yname);
		if (labels.Has(var1)){
			const auto& label = labels.Get(var1);
			if (label.size()==xvals.size()){
				localOpt->Set<bool>("xbinlabel",true);
				localOpt->Set<vector<string>>("xlabels",label);
			}
		}
		if (labels.Has(var2)){
			const auto& label = labels.Get(var2);
			if (label.size()==yvals.size()){
				localOpt->Set<bool>("ybinlabel",true);
				localOpt->Set<vector<string>>("ylabels",label);
			}
		}

		//make plot
		stringstream soname;
		soname << plotpre << "_" << sname << "_vs_" << var1 << "_" << var2;
		if(do_obs and !show_exp) soname << "_obs";
		if(!printsuffix.empty()) soname << "_" << printsuffix;
		string oname = soname.str();
		KParser::clean(oname);
		if(!dir.empty()) oname = dir+"/"+oname;
		KPlot2D* plot = new KPlot2D(oname,"",localOpt,plotOpt);
		//make histo for axes
		plot->Initialize();
		auto hbase = plot->GetHisto();
		hbase->GetZaxis()->SetRangeUser(zmin,zmax);
		hbase->GetZaxis()->SetTitle(zname.c_str());
		TCanvas* can = plot->GetCanvas();
		TPad* pad1 = plot->GetPad1();

		//make legend
		KLegend* kleg = new KLegend(pad1,localOpt,plotOpt);
		if(!region_text.empty()) kleg->AddEntry((TObject*)NULL,region_text,"");
		if(do_obs and !contours_obs.empty()) kleg->AddEntry(contours_obs[0],obs_text,"l");
		if(!contours_cen.empty()) kleg->AddEntry(contours_cen[0],med_text,"l");
		if(nsigma>=1) kleg->AddEntry(contours_sigmas[0][0],one_text,"l");
		if(nsigma>=2) kleg->AddEntry(contours_sigmas[2][0],two_text,"l");
		stringstream vname;
		for(const auto& v : vars){
			if(v.first==var1 or v.first==var2) continue;
			vname << vdict[v.first] << " = ";
			if(v.first=="alpha" and v.second<0) vname << alphaName(v.second);
			else vname << v.second;
			vname << erase(unitdict[v.first],{"[","]"});
			vname << ", ";
		}
		string svname = vname.str();
		if(!svname.empty()){
			svname.pop_back(); svname.pop_back();
			svname = "#lower[-0.1]{"+svname+"}";
			kleg->AddEntry((TObject*)NULL,svname,"");
		}
		//add all graphs to legend
		for(auto& v: {contours_obs,contours_cen}){
			for(auto c: v){
				kleg->AddGraph(c);
			}
		}
		for(auto& v: contours_sigmas){
			for(auto c: v){
				kleg->AddGraph(c);
			}
		}
		kleg->AddHist(plot->GetHisto()); //for tick sizes

		//use graph "best" algo (must be after axes drawn)
		if(use_yoff) hbase->GetYaxis()->SetTitleOffset(yoff);
		plot->DrawHist();
		kleg->Build(KLegend::hdefault,KLegend::vdefault);
		//extra style options
		auto leg = kleg->GetLegend();
		leg->SetFillStyle(1001);
		leg->SetFillColorAlpha(0,0.6);

		//draw objects
		h2d->Draw("colz same");
		for(const auto& v: contours_sigmas){
			for(auto c: v){
				c->Draw("c same");
			}
		}
		for(auto c: contours_cen){
			c->Draw("c same");
		}
		if(do_obs){
			for(auto c: contours_obs){
				c->Draw("c same");
			}
		}
		plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
		kleg->Draw();
		plot->DrawText();

		//print image
		//save directly to pdf: saving to eps and then converting to pdf loses transparent legend behavior
		can->Print((plot->GetName()+".pdf").c_str(),"pdf");
		//convert from pdf to png (better display of dashed lines)
		system(("convert -trim -density 300 "+plot->GetName()+".pdf "+plot->GetName()+".png").c_str());

		//save objects
		auto outfile = TFile::Open((plot->GetName()+".root").c_str(),"RECREATE");
		outfile->cd();
		hbase->Write();
		h2d->Write();
		//write contours w/ unique names
		if(!contours_cen.empty()) writeContour("central", contours_cen);
		if(do_obs and !contours_obs.empty()) writeContour("obs", contours_obs);
		vector<string> cnames{"sigma1d","sigma1u","sigma2d","sigma2u"};
		for(unsigned i = 0; i < contours_sigmas.size(); ++i){
			writeContour(cnames[i], contours_sigmas[i]);
		}
	}
}
