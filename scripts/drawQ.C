//ROOT headers
#include <TMath.h>
#include <TF1.h>
#include <TLine.h>

//custom headers
#include "Analysis/KCode/KMap.h"
#include "Analysis/KCode/KLegend.h"
#include "Analysis/KCode/KPlot.h"

//STL headers
#include <vector>
#include <string>

using namespace std;
using namespace TMath;

//--------------------------------------------------
//functions to initialize common drawing options
OptionMap* initGlobalOpt(){
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<double>("sizeLeg",21.5);
	globalOpt->Set<double>("sizeSymb",0.15);
	return globalOpt;
}
OptionMap* initLocalOpt(){
	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	return localOpt;
}

//---------------------------------------------------
//function to draw novosibirsk and gaussian functions
void drawQ(){
	//create base histo for drawing axes
	TH1F* hbase = new TH1F("base","",100,0,200);
	double ymin = 1.e-5;
	double ymax = 1.e-1;
	hbase->GetYaxis()->SetRangeUser(ymin,ymax);
	hbase->GetXaxis()->SetTitle("Events");
	hbase->GetYaxis()->SetTitle("PDF");

	//make plot options
	OptionMap* globalOpt = initGlobalOpt();
	OptionMap* localOpt = initLocalOpt();
	
	//make plot
	KPlot* plot = new KPlot("Q",localOpt,globalOpt);
	plot->Initialize(hbase);
	KLegend* kleg = plot->GetLegend();
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();

	//setup functions
	double n_b = 53;
	double n_sb = 104;
	TF1* f_b = new TF1("b","gaus",hbase->GetXaxis()->GetXmin(),hbase->GetXaxis()->GetXmax());
	f_b->SetParameters(1./Sqrt(2*Pi()*n_b),n_b,Sqrt(n_b));
	TF1* f_sb = new TF1("sb","gaus",hbase->GetXaxis()->GetXmin(),hbase->GetXaxis()->GetXmax());
	f_sb->SetParameters(1./Sqrt(2*Pi()*n_sb),n_sb,Sqrt(n_sb));
	
	//formatting
	f_b->SetLineWidth(2);
	f_b->SetLineColor(kRed);
	f_b->SetLineStyle(1);
	f_sb->SetLineWidth(2);
	f_sb->SetLineColor(kBlue);
	f_sb->SetLineStyle(1);
	
	//add to legend
	kleg->AddHist(hbase);
	kleg->AddEntry(f_b,"Background","l",0);
	kleg->AddEntry(f_sb,"Signal + Background","l",0);
	//build legend
	kleg->Build(KLegend::right,KLegend::top);

	//draw blank histo for axes
	plot->DrawHist();
	
	//draw functions
	kleg->Draw();
	f_b->Draw("same");
	f_sb->Draw("same");
	
	//extra stuff
	double intersect = Sqrt(n_b)*Sqrt(n_sb) + 0.5;
	TLine* line = new TLine(intersect, ymin, intersect, ymax);
	line->SetLineWidth(2);
	line->SetLineColor(kBlack);
	line->SetLineStyle(2);
	line->Draw("same");
	
	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	
	can->Print("Q_logy.png","png");
}