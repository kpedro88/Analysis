//custom headers
#include "KCode/KMap.h"
#include "KCode/KPlot.h"

//ROOT headers
#include <TFile.h>
#include <TTree.h>
#include <TH2.h>

//STL headers
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

map<string,string> legnames = {
	{"isrunc", "ISR uncertainty [%]"},
	{"JEC", "JEC uncertainty [%]"},
	{"JER", "JER uncertainty [%]"},
	{"MCStatErr", "MC statistical uncertainty [%]"},
	{"MHTSyst", "H_{T}^{miss} modeling uncertainty [%]"},
	{"puaccunc", "pileup acceptance uncertainty [%]"},
	{"puunc", "pileup reweighting uncertainty [%]"},
	{"scaleunc", "#mu_{R}/#mu_{F} uncertainty [%]"},
	{"trigunc", "trigger uncertainty [%]"},
	{"trigfnunc", "trigger uncertainty [%]"},
	{"prefireunc","L1 prefiring weight uncertainty [%]"},
	{"hemvetounc","HEM veto uncertainty [%]"},
	{"contam","SL contamination [%]"},
	{"pdfallunc", "PDF uncertainty [%]"},
	{"psisrunc", "PS ISR uncertainty [%]"},
	{"psfsrunc", "PS FSR uncertainty [%]"},
	{"JES", "JES uncertainty [%]"},
	{"JESfactor", "JES factor (J_{1,2})"},
	{"JESfactor1", "JES factor (J_{1})"},
	{"JESfactor2", "JES factor (J_{2})"},
	{"mMother", "M_{mother} [GeV]"},
	{"mLSP", "M_{LSP} [GeV]"},
	{"deltaM", "#DeltaM_{mother,LSP} [GeV]"},
	{"mZprime", "m_{Z'} [GeV]"},
	{"mDark", "m_{dark} [GeV]"},
	{"rinv", "r_{inv}"},
	{"alpha", "#alpha_{dark}"},
};

void getBinsQty(string rqty, TTree* tree, double& min, double& max, int& nbins, bool isMassPlane){
	if(rqty=="mZprime"){
		min = 1400; max = 5200; nbins = 19;
	}
	else if(rqty=="mDark"){
		min = -4; max = 106; nbins = 11;
	}
	else if(rqty=="rinv"){
		min = -0.05; max = 1.05; nbins = 11;
	}
	else if(rqty=="alpha"){
		min = -3.5; max = -0.5; nbins = 3;
	}
	else {
		min = 0; max = tree->GetMaximum(rqty.c_str()); nbins = 40;
	}

	if(isMassPlane){
		min = tree->GetMinimum(rqty.c_str());
		nbins = (max-min)/25;
	}
}

//make 2D plot
void plotSyst(TTree* tree, string xqty, string yqty, string model, string outdir, vector<string> printformats){
	//get ranges (use mMother for derived quantity deltaM)
	string rxqty = (xqty=="deltaM") ? "mMother" : xqty;
	string ryqty = (yqty=="deltaM") ? "mMother" : yqty;
	int nbinsx = 0, nbinsy = 0;
	double xmin = 0, xmax = 0, ymin = 0, ymax = 0;
	bool isPlane = false;
	bool isMassPlane = false;
	string cutPlane = "";
	if(xqty=="massPlane"){
		isPlane = isMassPlane = true;
		rxqty = "mMother";
		ryqty = "mLSP";
	}
	else if(xqty=="SVJPlane1"){
		isPlane = true;
		rxqty = "mZprime";
		ryqty = "mDark";
		cutPlane = "*(rinv==0.3&&alpha==-2.)";
	}
	else if(xqty=="SVJPlane2"){
		isPlane = true;
		rxqty = "mZprime";
		ryqty = "rinv";
		cutPlane = "*(mDark==20&&alpha==-2.)";
	}
	else if(xqty=="SVJPlane3"){
		isPlane = true;
		rxqty = "mZprime";
		ryqty = "alpha";
		cutPlane = "*(rinv==0.3&&mDark==20)";
	}

	getBinsQty(rxqty, tree, xmin, xmax, nbinsx, isMassPlane);
	getBinsQty(ryqty, tree, ymin, ymax, nbinsy, isMassPlane);

	//make name
	string hname = "plotSyst2D_"+yqty+"_vs_"+xqty+"_"+model;
	
	//initialize histos
	TH2F* hbase = new TH2F("hbase","",nbinsx,xmin,xmax,nbinsy,ymin,ymax);
	if(isPlane){
		hbase->GetXaxis()->SetTitle(legnames[rxqty].c_str());
		hbase->GetYaxis()->SetTitle(legnames[ryqty].c_str());
		hbase->GetZaxis()->SetTitle(legnames[yqty].c_str());		
	}
	else {
		hbase->GetXaxis()->SetTitle(legnames[xqty].c_str());
		hbase->GetYaxis()->SetTitle(legnames[yqty].c_str());
		hbase->GetZaxis()->SetTitle((model+" model points").c_str());
	}
	TH2F* hfill = (TH2F*)hbase->Clone(hname.c_str());
	TH2F* hfill_unweighted = (TH2F*)hbase->Clone((hname+"_unweighted").c_str());
	
	//draw into histo
	string drawname = yqty+":"+xqty+">>"+hname;
	string cutname = "";
	if(isPlane){
		drawname = ryqty+":"+rxqty+">>"+hname;
		cutname = yqty+cutPlane;
	}
	tree->Draw(drawname.c_str(),cutname.c_str(),"colz goff");
	if(isPlane) {
		//if some bins have multiple entries, take average (better than sum...)
		tree->Draw((drawname+"_unweighted").c_str(),("1"+cutPlane).c_str(),"colz goff");
		hfill->Divide(hfill_unweighted);
	}

	string lumi_text;
	if(isPlane) lumi_text = "13 TeV ("+model+")";
	else {
		double correl = hfill->GetCorrelationFactor();
		stringstream ss;
		ss << fixed << setprecision(2);
		ss << "13 TeV (r = " << correl << ")";
		lumi_text = ss.str();
	}
	
	//setup plot
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<string>("lumi_text",lumi_text);
	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	if(isPlane and hfill->GetMaximum()<=110) localOpt->Set<bool>("logz",false);
	KPlot2D* plot = new KPlot2D(hname,"",localOpt,globalOpt);
	plot->Initialize(hbase);
	
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();
	plot->DrawHist();
	hfill->Draw("colz same");
	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();
	
	//print image
	for(auto pformat : printformats){
		bool epstopdf = pformat=="pdf";
		if(epstopdf) pformat = "eps";
		string oname = outdir+"/"+hname+"."+pformat;
		can->Print(oname.c_str(),pformat.c_str());
		if(epstopdf) system(("epstopdf "+oname+" && rm "+oname).c_str());
	}
}

//definitely only run in batch mode
void plotSyst2D(string indir, 
				vector<string> models={"T1bbbb","T1qqqq","T1tbtb","T1tbtbT1tbbbT1bbbb","T1tbtbT1tbttT1tttt","T1ttbb","T1tttt","T2bb","T2qq","T2tt","T5qqqqVV"}, 
				vector<string> systs={"isrunc","JEC","JER","MCStatErr","MHTSyst","puaccunc","scaleunc","trigunc"},
				vector<string> compares={"mMother","mLSP","deltaM","massPlane"},
				bool correlate=true,
				string outdir="plots/syst",
				string prefix="tree_syst",
				string suffix="fast",
				vector<string> printformats={"png"})
{
	system(("mkdir -p "+outdir).c_str());

	//explore correlations among all systs
	if(correlate){
		compares.insert(compares.end(),systs.begin(),systs.end());
	}
	
	for(const auto& model : models){
		//get file/tree
		string fname = indir+"/"+prefix+"_"+model+"_"+suffix+".root";
		TFile* file = KOpen(fname);
		TTree* tree = KGet<TTree>(file,"tree");
		tree->SetAlias("deltaM","mMother-mLSP");
		cout << model << endl;
		std::set<string> compared;
		
		//loop over systs and compares
		for(const auto& syst : systs){
			cout << "\t" << syst << endl;
			compared.insert(syst+"_"+syst);
			for(const auto& compare : compares){
				//avoid self-compares and double counting
				if(compared.find(syst+"_"+compare)!=compared.end() or compared.find(compare+"_"+syst)!=compared.end()) continue;
				cout << "\t\t" << compare << endl;
				compared.insert(syst+"_"+compare);
				//make plot
				plotSyst(tree,compare,syst,model,outdir,printformats);
			}
		}
		file->Close();
	}
}
