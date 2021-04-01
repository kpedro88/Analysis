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
	Curve(KNamed* tmp, OptionMap* globalOpt){
		localOpt = tmp->localOpt();
		name = tmp->fields[0];
		localOpt->Get("legname",legname);
		panel = 0; localOpt->Get("panel",panel);
		localOpt->Get("extra_text",extra_text);
		style = new KStyle(name,globalOpt,localOpt);

		string extfilename; localOpt->Get("extfilename",extfilename);
		//get graph
		if(!extfilename.empty()){
			auto extfile = KOpen(extfilename);
			string extgraph; localOpt->Get("extgraph",extgraph);
			graph = KGet<TGraph>(extfile,extgraph);
			style->Format(graph);
		}
		//make graph
		else {
			vector<double> xvals, yvals, xsecs;
			globalOpt->Get("xvals",xvals);
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
		ymin = TMath::MinElement(graph->GetN(),graph->GetY());
		ymax = TMath::MaxElement(graph->GetN(),graph->GetY());
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
};

void compareLimits(string oname, string input, string options){
	OptionMap* globalOpt = new OptionMap();
	KParser::processOption("in:input["+options+"]",globalOpt);

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
		curve.AddToLegend(kleg);
		if(curve.ymin<ymin) ymin = curve.ymin;
		if(curve.ymax>ymax) ymax = curve.ymax;
	}
	//set extrema
	double yfactor = 10; globalOpt->Get("yfactor",yfactor);
	hbase->GetYaxis()->SetRangeUser(ymin/yfactor,ymax*yfactor);

	//draw blank histo for axes
	plot->DrawHist();
	
	//use graph "best" algo (must be after axes drawn)
	kleg->Build(KLegend::hdefault,KLegend::vdefault);
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
}

