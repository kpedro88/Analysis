//analysis headers
#include "Analysis/KCode/KMap.h"
#include "Analysis/KCode/KLegend.h"
#include "Analysis/KCode/KPlot.h"
#include "Analysis/KCode/KParser.h"
#include "Analysis/KCode/KStyle.h"

//ROOT headers
#include <TH1.h>
#include <TF1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>

//STL headers
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

void doubleRatio(string mcpre, string datapre, string srname, string crname, string fnname){
	auto file_mc = KOpen(mcpre+srname+","+crname+".root");
	auto file_data = KOpen(datapre+srname+","+crname+".root");

	//divide histos
	auto h_mc = KGet<TH1>(file_mc,"MTAK8_ratio__"+srname+"__"+crname);
	auto h_data = KGet<TH1>(file_data,"MTAK8_ratio__"+srname+"__"+crname);
	h_data->Divide(h_mc);
	double xmin = h_mc->GetXaxis()->GetXmin();
	double xmax = h_mc->GetXaxis()->GetXmax();

	//divide functions
	auto f_mc = KGet<TF1>(file_mc,fnname+"_"+h_mc->GetName());
	auto f_data = KGet<TF1>(file_data,fnname+"_"+h_data->GetName());

	//use function expressions w/ param values to construct data/mc
	TF1* f_dratio = new TF1("dratio",("("+f_data->GetExpFormula("P")+")/("+f_mc->GetExpFormula("P")+")"),xmin,xmax);

	//set up plot
	TH1F* hbase = new TH1F("hbase","",100,xmin,xmax);
	string xtitle = "M_{T} [GeV]";
	string ytitle = "TF_{data}/TF_{mc}";
	hbase->GetXaxis()->SetTitle(xtitle.c_str());
	hbase->GetYaxis()->SetTitle(ytitle.c_str());
	hbase->GetYaxis()->SetRangeUser(0,2);
	string oname = "dratio_"+srname+","+crname;
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<double>("luminorm",137200);
	globalOpt->Set<string>("prelim_text","Work-in-progress");
	globalOpt->Set<bool>("logy",false);
	globalOpt->Set<bool>("ratio",false);
	KPlot* plot = new KPlot(oname,globalOpt,globalOpt);
	plot->Initialize(hbase);
	KLegend* kleg = plot->GetLegend();
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();
	kleg->AddHist(plot->GetHisto()); //for tick sizes
	kleg->AddEntry(NULL,srname+", "+crname,"");
	kleg->AddHist(h_data);

	//draw blank histo for axes
	plot->DrawHist();
	
	//use graph "best" algo (must be after axes drawn)
	kleg->Build(KLegend::hdefault,KLegend::vdefault);

	//draw ratios
	h_data->Draw("p same");
	f_dratio->Draw("l same");

	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();
	
	//print image
	vector<string> pformats{"png","pdf"};
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
