//analysis headers
#include "Analysis/KCode/KMap.h"
#include "Analysis/KCode/KLegend.h"
#include "Analysis/KCode/KPlot.h"
#include "Analysis/KCode/KParser.h"
#include "Analysis/KCode/KStyle.h"

//ROOT headers
#include <TH1.h>
#include <TMath.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>

//STL headers
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

//other headers
#include <boost/range/adaptor/reversed.hpp>

using namespace std;

struct Curve {
	Curve(KNamed* tmp, OptionMap* globalOpt) : ymin(1e10), ymax(-1e10) {
		localOpt = tmp->localOpt();
		name = tmp->fields[0];
		localOpt->Get("legname",legname);
		panel = 0; localOpt->Get("panel",panel);
		localOpt->Get("extra_text",extra_text);
		style = new KStyle(name,globalOpt,localOpt);
		obs = localOpt->Get("obs",false);
		skiplegend = localOpt->Get("skiplegend",false);

		string extfilename; localOpt->Get("extfilename",extfilename);
		//get graph
		if(!extfilename.empty()){
			auto extfile = KOpen(extfilename);
			string extgraph; localOpt->Get("extgraph",extgraph);
			graph = KGet<TGraph>(extfile,extgraph);
			//option to multiply graph by something
			vector<double> xsecs;
			bool has_xsec = globalOpt->Get("xsecs",xsecs);
			if (has_xsec) {
				if(xsecs.size()!=graph->GetN()) throw std::runtime_error("Different lengths: "+to_string(xsecs.size())+" (xsec), "+to_string(graph->GetN())+" (graph)");
				for(unsigned i = 0; i < graph->GetN(); ++i){
					graph->GetY()[i] = graph->GetY()[i]*xsecs[i];
				}
			}
			//option to completely remove points outside of specified y bounds
			if(globalOpt->Get("ytruncate",false)){
				bool manual_ymin = globalOpt->Get("ymin",ymin);
				bool manual_ymax = globalOpt->Get("ymax",ymax);
				vector<double> xvals, yvals;
				for(unsigned i = 0; i < graph->GetN(); ++i){
					if((manual_ymin and graph->GetY()[i] < ymin) or (manual_ymax and graph->GetY()[i] > ymax)) continue;
					xvals.push_back(graph->GetX()[i]);
					yvals.push_back(graph->GetY()[i]);
				}
				graph = new TGraph(xvals.size(),xvals.data(),yvals.data());
			}
			style->Format(graph);
		}
		//make graph
		else {
			vector<double> xvals, yvals, xsecs;
			globalOpt->Get("xvals",xvals);
			localOpt->Get("xvals",xvals);
			bool has_xsec = globalOpt->Get("xsecs",xsecs);
			localOpt->Get("yvals",yvals);
			if (has_xsec) {
				for(unsigned i = 0; i < yvals.size(); ++i){
					yvals[i] = yvals[i]*xsecs[i];
				}
			}
			graph = new TGraph(xvals.size(),xvals.data(),yvals.data());
			style->Format(graph);
		}

		//get extrema
		if(graph->GetN()>0){
			ymin = TMath::MinElement(graph->GetN(),graph->GetY());
			ymax = TMath::MaxElement(graph->GetN(),graph->GetY());
		}
	}
	//helpers
	void AddToLegend(KLegend* kleg){
		kleg->AddEntry(graph,legname,style->GetLegOpt(),panel,extra_text);
	}
	void Draw(){
		graph->Draw(style->GetDrawOpt("same").c_str());
	}
	//members
	string name;
	OptionMap* localOpt;
	string legname;
	int panel;
	vector<string> extra_text;
	TGraph* graph;
	KStyle* style;
	double ymin, ymax;
	bool obs;
	bool skiplegend;
};

void compareLimits(string oname, string input, string options, vector<string> more_options={}){
	OptionMap* globalOpt = new OptionMap();
	KParser::processOption("in:input["+options+"]",globalOpt);
	for(const auto& opt : more_options){
		KParser::processOption(opt,globalOpt);
	}
	bool do_obs = globalOpt->Get("do_obs",false);
	auto legh = KLegend::hdefault;
	auto legv = KLegend::vdefault;
	string slegh; globalOpt->Get("slegh",slegh);
	if(slegh.size()>0){
		if(slegh=="left") legh = KLegend::left;
		else if(slegh=="center") legh = KLegend::center;
		else if(slegh=="right") legh = KLegend::right;
	}
	string slegv; globalOpt->Get("slegv",slegv);
	if(slegv.size()>0){
		if(slegv=="top") legv = KLegend::top;
		else if(slegv=="middle") legv = KLegend::middle;
		else if(slegv=="bottom") legv = KLegend::bottom;
	}

	//set up plot
	vector<double> xvals; globalOpt->Get("xvals",xvals);
	TH1F* hbase = new TH1F("hbase","",100,xvals.front(),xvals.back());
	string xtitle; globalOpt->Get("xtitle",xtitle);
	string ytitle; globalOpt->Get("ytitle",ytitle);
	hbase->GetXaxis()->SetTitle(xtitle.c_str());
	hbase->GetYaxis()->SetTitle(ytitle.c_str());
	KPlot* plot = new KPlot(oname,globalOpt,globalOpt);
	plot->Initialize(hbase);
	KLegend* kleg = plot->GetLegend();
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();
	kleg->AddHist(plot->GetHisto()); //for tick sizes

	//make curves, add to legend, check extrema
	vector<KNamed*> tmps = KParser::processStandalone(input);
	double ymin = 1e100;
	double ymax = 0;
	vector<Curve> curves; curves.reserve(tmps.size());
	for(const auto& tmp : tmps){
		curves.emplace_back(tmp, globalOpt);
		auto& curve = curves.back();
		if(!do_obs and curve.obs){
			curves.pop_back();
			continue;
		}
		if(!curve.skiplegend) curve.AddToLegend(kleg);
		if(curve.ymin<ymin) ymin = curve.ymin;
		if(curve.ymax>ymax) ymax = curve.ymax;
	}
	bool manual_ymin = globalOpt->Get("ymin",ymin);
	bool manual_ymax = globalOpt->Get("ymax",ymax);
	//set extrema
	double yfactor = 10; globalOpt->Get("yfactor",yfactor);
	hbase->GetYaxis()->SetRangeUser(manual_ymin ? ymin : ymin/yfactor, manual_ymax ? ymax : ymax*yfactor);

	//draw blank histo for axes
	plot->DrawHist();
	
	//use graph "best" algo (must be after axes drawn)
	kleg->Build(legh,legv);
	//extra style options
	auto leg = kleg->GetLegend();
	leg->SetFillStyle(1001);
	leg->SetFillColorAlpha(0,0.6);

	//draw graphs
	if(globalOpt->Get("reverse",false)){
		for(auto& curve : boost::adaptors::reverse(curves)){
			curve.Draw();
		}
	}
	else {
		for(auto& curve : curves){
			curve.Draw();
		}
	}

	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();

	string dir; globalOpt->Get("dir",dir);
	//print image
	//save directly to pdf: saving to eps and then converting to pdf loses transparent legend behavior
	vector<string> pformats; globalOpt->Get("printformat",pformats);
	for(auto pformat : pformats){
		string otmp = oname+"."+pformat;
		KParser::clean(otmp);
		if(!dir.empty()) otmp = dir+"/"+otmp;
		//if possible, convert from pdf to png (better display of dashed lines)
		if(pformat=="png"){
			string otmp2 = oname+".pdf";
			KParser::clean(otmp2);
			if(!dir.empty()) otmp2 = dir+"/"+otmp2;
			ifstream f(otmp2.c_str());
			if(f.good()){
				system(("convert -trim -density 300 "+otmp2+" "+otmp).c_str());
				continue;
			}
		}
		can->Print(otmp.c_str(),pformat.c_str());
	}

	//save objects
	if(globalOpt->Get("save",false)){
		string otmp = oname+".root";
		KParser::clean(otmp);
		if(!dir.empty()) otmp = dir+"/"+otmp;
		auto outfile = TFile::Open(otmp.c_str(),"RECREATE");
		outfile->cd();
		for(auto& curve : curves){
			curve.graph->SetName(curve.name.c_str());
			curve.graph->Write();
		}
	}
}

