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
#include <utility>
#include <tuple>

using namespace std;

map<string,string> legnames = {
	{"isrunc", "ISR uncertainty [%]"},
	{"JEC", "JEC uncertainty [%]"},
	{"JER", "JER uncertainty [%]"},
	{"MCStatErr", "MC statistical uncertainty [%]"},
	{"MCStatOverallErr", "MC statistical uncertainty [%]"},
	{"MHTSyst", "H_{T}^{miss} modeling uncertainty [%]"},
	{"puaccunc", "pileup acceptance uncertainty [%]"},
	{"puunc", "pileup reweighting uncertainty [%]"},
	{"scaleunc", "#mu_{R}/#mu_{F} uncertainty [%]"},
	{"trigunc", "trigger uncertainty [%]"},
	{"trigfnunc", "trigger uncertainty [%]"},
	{"prefireunc","L1 prefiring weight uncertainty [%]"},
	{"hemvetounc","HEM veto uncertainty [%]"},
	{"contam","SL contamination [%]"},
	{"pdfunc", "PDF uncertainty [%]"},
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

vector<double> getVariableBins(const vector<double>& vals){
	vector<double> bins; bins.reserve(vals.size()+1);
	for(unsigned i = 1; i < vals.size(); ++i){
		//midpoint between bin centers
		double midpt = (vals[i]+vals[i-1])/2.;
		//starting point for first bin
		if(i==1) bins.push_back(2*vals[i-1]-midpt);
		bins.push_back(midpt);
		//ending point for last bin
		if(i==vals.size()-1) bins.push_back(2*vals[i]-midpt);
	}
	return bins;
}

pair<bool,vector<double>> getBinsQty(string rqty, TTree* tree, bool isMassPlane){
	//fixed-width case: min, max, nbins
	//variable-width case: bin boundaries (length nbins+1)
	vector<double> bin_info;
	bool fixed = true;
	if(rqty=="mZprime"){
		bin_info = {1400,5200,19};
	}
	else if(rqty=="mDark"){
		vector<double> vals{1,5,10,20,30,40,50,60,70,80,90,100};
		bin_info = getVariableBins(vals);
		fixed = false;
	}
	else if(rqty=="rinv"){
		vector<double> vals{0,0.05,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
		bin_info = getVariableBins(vals);
		fixed = false;
	}
	else if(rqty=="alpha"){
		bin_info = {-3.5,-0.5,3};
	}
	else {
		bin_info = {0, tree->GetMaximum(rqty.c_str()), 40};
	}

	if(isMassPlane){
		bin_info[0] = tree->GetMinimum(rqty.c_str());
		bin_info[2] = (bin_info[1]-bin_info[0])/25;
	}

	return make_pair(fixed,bin_info);
}

//make 2D plot
void plotSyst(TTree* tree, string xqty, string yqty, string model, string outdir, vector<string> printformats){
	//get ranges (use mMother for derived quantity deltaM)
	string rxqty = (xqty=="deltaM") ? "mMother" : xqty;
	string ryqty = (yqty=="deltaM") ? "mMother" : yqty;
	vector<double> binsx, binsy;
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

	bool fixedx,fixedy;
	tie(fixedx,binsx) = getBinsQty(rxqty, tree, isMassPlane);
	tie(fixedy,binsy) = getBinsQty(ryqty, tree, isMassPlane);

	//make name
	string hname = "plotSyst2D_"+yqty+"_vs_"+xqty+"_"+model;
	
	//initialize histos
	TH2F* hbase = nullptr;
	if(fixedx){
		if(fixedy){
			hbase = new TH2F("hbase","",binsx[2],binsx[0],binsx[1],binsy[2],binsy[0],binsy[1]);
		}
		else {
			hbase = new TH2F("hbase","",binsx[2],binsx[0],binsx[1],binsy.size()-1,binsy.data());
		}
	}
	else {
		if(fixedy){
			hbase = new TH2F("hbase","",binsx.size()-1,binsx.data(),binsy[2],binsy[0],binsy[1]);
		}
		else {
			hbase = new TH2F("hbase","",binsx.size()-1,binsx.data(),binsy.size()-1,binsy.data());
		}
	}
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
