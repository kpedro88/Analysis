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
	{"MHTSyst", "H_{T}^{miss,gen} uncertainty [%]"},
	{"puaccunc", "pileup acceptance uncertainty [%]"},
	{"scaleunc", "#mu_{R}/#mu_{F} uncertainty [%]"},
	{"trigunc", "trigger uncertainty [%]"},
	{"mMother", "M_{mother} [GeV]"},
	{"mLSP", "M_{LSP} [GeV]"},
	{"deltaM", "#DeltaM_{mother,LSP} [GeV]"},
};

//make 2D plot
void plotSyst(TTree* tree, string xqty, string yqty, string model, string outdir, vector<string> printformats){
	int nbinsx = 40, nbinsy = 40;
	//get ranges (use mMother for derived quantity deltaM)
	string rxqty = (xqty=="deltaM") ? "mMother" : xqty;
	string ryqty = (yqty=="deltaM") ? "mMother" : yqty;
	if(xqty=="massPlane"){
		rxqty = "mMother";
		ryqty = "mLSP";
	}
	double xmin = 0;
	double xmax = tree->GetMaximum(rxqty.c_str());
	double ymin = 0;
	double ymax = tree->GetMaximum(ryqty.c_str());
	if(xqty=="massPlane"){
		xmin = tree->GetMinimum(rxqty.c_str());		
		ymin = tree->GetMinimum(ryqty.c_str());
	}
	//make name
	string hname = "plotSyst2D_"+yqty+"_vs_"+xqty+"_"+model;
	
	//initialize histos
	if(xqty=="massPlane") {
		nbinsx = (xmax-xmin)/25;
		nbinsy = (ymax-ymin)/25;
	}
	TH2F* hbase = new TH2F("hbase","",nbinsx,xmin,xmax,nbinsy,ymin,ymax);
	if(xqty=="massPlane"){
		hbase->GetXaxis()->SetTitle(legnames["mMother"].c_str());
		hbase->GetYaxis()->SetTitle(legnames["mLSP"].c_str());
		hbase->GetZaxis()->SetTitle(legnames[yqty].c_str());		
	}
	else {
		hbase->GetXaxis()->SetTitle(legnames[xqty].c_str());
		hbase->GetYaxis()->SetTitle(legnames[yqty].c_str());
		hbase->GetZaxis()->SetTitle((model+" model points").c_str());
	}
	TH2F* hfill = (TH2F*)hbase->Clone(hname.c_str());
	
	//draw into histo
	string drawname = yqty+":"+xqty+">>"+hname;
	string cutname = "";
	if(xqty=="massPlane"){
		drawname = "mLSP:mMother>>"+hname;
		cutname = yqty;
	}
	tree->Draw(drawname.c_str(),cutname.c_str(),"colz goff");
	double correl = hfill->GetCorrelationFactor();
	stringstream ss;
	ss << fixed << setprecision(2);
	ss << "13 TeV (r = " << correl << ")";
	string lumi_text = ss.str();
	if(xqty=="massPlane") lumi_text = "13 TeV ("+model+")";
	
	//setup plot
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<string>("lumi_text",lumi_text);
	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	if(xqty=="massPlane") localOpt->Set<bool>("logz",false);
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
	for(const auto& printformat : printformats){
		can->Print((outdir+"/"+hname+"."+printformat).c_str(),printformat.c_str());
		if(printformat=="eps") system(("epstopdf "+hname+".eps").c_str());
	}
}

//definitely only run in batch mode
void plotSyst2D(string indir, 
				vector<string> models={"T1bbbb","T1qqqq","T1tbtb","T1tbtbT1tbbbT1bbbb","T1tbtbT1tbttT1tttt","T1ttbb","T1tttt","T2bb","T2qq","T2tt","T5qqqqVV"}, 
				vector<string> systs={"isrunc","JEC","JER","MCStatErr","MHTSyst","puaccunc","scaleunc","trigunc"},
				vector<string> compares={"mMother","mLSP","deltaM","massPlane"},
				bool correlate=true,
				string outdir="plots-syst",
				vector<string> printformats={"png"})
{
	//explore correlations among all systs
	if(correlate){
		compares.insert(compares.end(),systs.begin(),systs.end());
	}
	
	for(const auto& model : models){
		//get file/tree
		string fname = indir+"/tree_syst_"+model+"_fast.root";
		TFile* file = TFile::Open(fname.c_str());
		if(!file) {
			cout << "Error: syst tree for model " << model << " not found!" << endl;
			continue;
		}
		TTree* tree = (TTree*)file->Get("tree");
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
