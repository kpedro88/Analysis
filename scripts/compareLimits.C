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

using namespace std;

struct Curve {
	Curve(KNamed* tmp, OptionMap* globalOpt){
		localOpt = tmp->localOpt();
		name = tmp->fields[0];
		localOpt->Get("legname",legname);
		style = new KStyle(name,globalOpt,localOpt);

		//make graph
		vector<double> xvals, yvals, xsecs;
		globalOpt->Get("xvals",xvals);
		globalOpt->Get("xsecs",xsecs);
		localOpt->Get("yvals",yvals);
		for(unsigned i = 0; i < yvals.size(); ++i){
			yvals[i] = yvals[i]*xsecs[i];
		}
		graph = new TGraph(xvals.size(),xvals.data(),yvals.data());
		style->Format(graph);

		//get extrema
		ymin = TMath::MinElement(yvals.size(),yvals.data());
		ymax = TMath::MaxElement(yvals.size(),yvals.data());
	}
	//members
	string name;
	OptionMap* localOpt;
	string legname;
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
		const auto& curve = curves.back();
		kleg->AddEntry(curve.graph,curve.legname,curve.style->GetLegOpt());
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

	//draw graphs
	for(const auto& curve : curves){
		curve.graph->Draw(curve.style->GetDrawOpt("same").c_str());
	}

	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();
	
	//print image
	vector<string> pformats; globalOpt->Get("printformat",pformats);
	for(auto pformat : pformats){
		string otmp = oname;
		bool epstopdf = pformat=="pdf";
		if(epstopdf) pformat = "eps";
		otmp += "." + pformat;
		KParser::clean(otmp);
		can->Print(otmp.c_str(),pformat.c_str());
		if(epstopdf) system(("epstopdf "+otmp+" && rm "+otmp).c_str());
	}
}
